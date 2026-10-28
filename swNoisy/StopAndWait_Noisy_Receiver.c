#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
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

int main(void)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in receiver;
    memset(&receiver, 0, sizeof(receiver));
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(PORT);
    receiver.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&receiver, sizeof(receiver)) < 0) {
        perror("Bind failed");
        close(sock);
        return 1;
    }

    printf("Receiver ready on port %d. Press Ctrl+C to stop.\n", PORT);

    int configured = 0;
    uint32_t tt = 0, tp = 0, loss = 0;
    uint32_t expected = 1;
    int success_count = 0;
    time_t start_time = time(NULL);

#define ELAPSED() ((int)difftime(time(NULL), start_time))

    for (;;) {
        Msg m;
        struct sockaddr_in sender;
        socklen_t len = sizeof(sender);

        ssize_t n = recvfrom(sock, &m, sizeof(m), 0,
                             (struct sockaddr *)&sender, &len);
        if (n != (ssize_t)sizeof(m))
            continue;

        ntoh_msg(&m);

        switch (m.type) {

        case PKT_CONFIG:
            /* (Re)start a session */
            tt = m.tt;
            tp = m.tp;
            loss = m.loss;
            expected = 1;
            success_count = 0;
            configured = 1;
            start_time = time(NULL);
            printf("\n--- New session: tt=%u, tp=%u, loss=%u ---\n",
                   tt, tp, loss);
            send_msg(sock, &sender, PKT_CONFIG_ACK, 0, 0, 0, 0);
            break;

        case PKT_DATA: {
            if (!configured)
                break;

            /* Simulate propagation delay */
            sleep(tp);

            if (m.seq == expected) {
                printf("t%d: Received Packet %u from transmitter\n",
                       ELAPSED(), m.seq);
                expected++;
            } else if (m.seq < expected) {
                printf("t%d: Duplicate Packet %u received "
                       "(already delivered, re-sending ACK)\n",
                       ELAPSED(), m.seq);
            } else {
                printf("t%d: Out-of-order Packet %u (expected %u) - ignored\n",
                       ELAPSED(), m.seq, expected);
                break;
            }

            /* Simulate transmission/processing delay */
            sleep(tt);

            /* Original ACK-loss logic */
            int drop = 0;
            if (loss > 0 && success_count == (int)loss - 1) {
                drop = 1;
                success_count = 0;
            }

            if (drop) {
                printf("t%d: ACK not sent for Packet %u (simulated loss)\n",
                       ELAPSED(), m.seq);
            } else {
                send_msg(sock, &sender, PKT_ACK, m.seq, 0, 0, 0);
                printf("t%d: ACK sent for Packet %u\n", ELAPSED(), m.seq);
                success_count++;
            }
            break;
        }

        case PKT_FIN:
            if (configured) {
                printf("t%d: Sender finished. Waiting for next session...\n",
                       ELAPSED());
                configured = 0;
            }
            break;

        default:
            break;
        }
    }

    close(sock);
    return 0;
}
