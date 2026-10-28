#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9000

enum {
    PKT_CONFIG     = 1,
    PKT_CONFIG_ACK = 2,
    PKT_DATA       = 3,
    PKT_ACK        = 4,
    PKT_FIN        = 5
};

typedef struct {
    uint32_t type;
    uint32_t seq;
    uint32_t tt;
    uint32_t tp;
    uint32_t loss;
} Msg;

static int send_msg(int sock, const struct sockaddr_in *to,
                    uint32_t type, uint32_t seq,
                    uint32_t tt, uint32_t tp, uint32_t loss)
{
    Msg m;
    m.type = htonl(type);
    m.seq  = htonl(seq);
    m.tt   = htonl(tt);
    m.tp   = htonl(tp);
    m.loss = htonl(loss);
    return (int)sendto(sock, &m, sizeof(m), 0,
                       (const struct sockaddr *)to, sizeof(*to));
}

static void ntoh_msg(Msg *m)
{
    m->type = ntohl(m->type);
    m->seq  = ntohl(m->seq);
    m->tt   = ntohl(m->tt);
    m->tp   = ntohl(m->tp);
    m->loss = ntohl(m->loss);
}

static time_t start_time;
#define ELAPSED() ((int)difftime(time(NULL), start_time))

static long now_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

/* Read an integer in [min, max] with validation */
static int read_int(const char *prompt, long min, long max)
{
    char line[64];
    for (;;) {
        printf("%s", prompt);
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) {
            fprintf(stderr, "\nInput closed.\n");
            exit(1);
        }
        char *end;
        errno = 0;
        long v = strtol(line, &end, 10);
        if (end == line || errno != 0 ||
            (*end != '\n' && *end != '\0') || v < min || v > max) {
            printf("Invalid input. Enter an integer between %ld and %ld.\n",
                   min, max);
            continue;
        }
        return (int)v;
    }
}

/* Wait for a message of the given type and seq.
   Returns 1 = received, 0 = timeout, -1 = error.
   Stale/unrelated messages are ignored without extending the deadline. */
static int wait_for_msg(int sock, uint32_t want_type, uint32_t want_seq,
                        int timeout_ms)
{
    long deadline = now_ms() + timeout_ms;

    for (;;) {
        long remaining = deadline - now_ms();
        if (remaining <= 0)
            return 0;

        struct pollfd pfd = { sock, POLLIN, 0 };
        int r = poll(&pfd, 1, (int)remaining);
        if (r < 0) {
            if (errno == EINTR) continue;
            perror("poll failed");
            return -1;
        }
        if (r == 0)
            return 0;

        Msg m;
        ssize_t n = recvfrom(sock, &m, sizeof(m), 0, NULL, NULL);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("recvfrom failed");
            return -1;
        }
        if (n != (ssize_t)sizeof(m))
            continue;

        ntoh_msg(&m);
        if (m.type == want_type && m.seq == want_seq)
            return 1;
        /* otherwise: stale or unrelated, ignore */
    }
}

int main(void)
{
    int size = read_int("Enter number of packets to transmit: ", 1, 100000);
    int tt   = read_int("Enter Transmission Time (sec): ", 0, 100);
    int tp   = read_int("Enter Propagation Time (sec): ", 0, 100);

    int loss;
    for (;;) {
        loss = read_int("Enter Loss value (0 = no loss, N = every Nth ACK "
                        "lost after N-1 successes): ", 0, 1000);
        if (loss == 1) {
            printf("Loss value 1 would drop every ACK (never finishes). "
                   "Use 0 or a value >= 2.\n");
            continue;
        }
        break;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in receiver;
    memset(&receiver, 0, sizeof(receiver));
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(PORT);
    receiver.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Original timing formula */
    int rtt = 2 * tt + 2 * tp;
    int timeout_sec = 3 * rtt;
    if (timeout_sec < 1)
        timeout_sec = 1;   /* avoid a zero timeout */

    /* Handshake: send config, wait for confirmation (up to 5 tries) */
    int connected = 0;
    for (int attempt = 1; attempt <= 5 && !connected; attempt++) {
        if (send_msg(sock, &receiver, PKT_CONFIG, 0, tt, tp, loss) < 0) {
            perror("sendto (config) failed");
            close(sock);
            return 1;
        }
        int r = wait_for_msg(sock, PKT_CONFIG_ACK, 0, 2000);
        if (r < 0) { close(sock); return 1; }
        connected = (r == 1);
    }
    if (!connected) {
        fprintf(stderr, "Receiver not responding. Start the receiver first.\n");
        close(sock);
        return 1;
    }

    printf("\nRTT = 2*%d + 2*%d = %d sec, Timeout = 3*RTT = %d sec\n\n",
           tt, tp, rtt, timeout_sec);

    start_time = time(NULL);

    int seq = 1;
    int retransmit = 0;

    while (seq <= size) {
        /* Simulate transmission time */
        sleep(tt);

        printf("t%d: Packet %d sent to receiver%s\n",
               ELAPSED(), seq, retransmit ? " (retransmission)" : "");

        if (send_msg(sock, &receiver, PKT_DATA, seq, 0, 0, 0) < 0) {
            perror("sendto (data) failed");
            close(sock);
            return 1;
        }

        int r = wait_for_msg(sock, PKT_ACK, seq, timeout_sec * 1000);

        if (r < 0) {
            close(sock);
            return 1;
        } else if (r == 1) {
            /* Simulate ACK propagation time */
            sleep(tp);
            printf("t%d: Acknowledgement for Packet %d received from receiver\n",
                   ELAPSED(), seq);
            seq++;
            retransmit = 0;
        } else {
            printf("t%d: Timeout (waited %d sec = 3*RTT) - "
                   "Acknowledgement not received for Packet %d\n",
                   ELAPSED(), timeout_sec, seq);
            printf("t%d: Retransmitting Packet %d\n", ELAPSED(), seq);
            retransmit = 1;
        }
    }

    send_msg(sock, &receiver, PKT_FIN, 0, 0, 0, 0);

    printf("t%d: Transmission completed\n", ELAPSED());
    printf("Total time required: %d seconds\n", ELAPSED());

    close(sock);
    return 0;
}
