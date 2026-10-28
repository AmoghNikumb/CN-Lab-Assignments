#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    int sock;
    struct sockaddr_in receiver;
    int size = 0, tp, tt, loss = 0;

    printf("Enter number of packets to transmit: ");
    scanf("%d", &size);
    printf("Enter Transmission Time (sec): ");
    scanf("%d", &tt);
    printf("Enter Propagation Time (sec): ");
    scanf("%d", &tp);
    printf("Enter Loss value (0 = no loss, N = every Nth packet lost after N-1 successes): ");
    scanf("%d", &loss);

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(9000);
    receiver.sin_addr.s_addr = inet_addr("127.0.0.1");

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    int rtt = 2 * tt + 2 * tp;
    int timeout_sec = 3 * rtt;

    //struct timeval sock_timeout;
    //sock_timeout.tv_sec = timeout_sec;
    //sock_timeout.tv_usec = 0;
    //setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &sock_timeout, sizeof(sock_timeout));

    sendto(sock, &tt, sizeof(tt), 0, (struct sockaddr *)&receiver, sizeof(receiver));
    sendto(sock, &tp, sizeof(tp), 0, (struct sockaddr *)&receiver, sizeof(receiver));
    sendto(sock, &loss, sizeof(loss), 0, (struct sockaddr *)&receiver, sizeof(receiver));

    int buffer = 1, ack = 0;
    socklen_t len = sizeof(receiver);
    time_t start_time = time(NULL);

    do {
        sleep(tt);
        printf("t%d: Packet %d sent to receiver\n", (int)difftime(time(NULL), start_time), buffer);
        sendto(sock, &buffer, sizeof(buffer), 0, (struct sockaddr *)&receiver, sizeof(receiver));

        int n = recvfrom(sock, &ack, sizeof(ack), 0, (struct sockaddr *)&receiver, &len);

        if (n > 0) {
            sleep(tp);
            printf("t%d: Acknowledgement for Packet %d received from receiver\n", (int)difftime(time(NULL), start_time), buffer);
            buffer++;
        } else {
            sleep(timeout_sec);
            printf("t%d: Timeout (waited %d sec = 3*RTT) - Acknowledgement not received for Packet %d\n", (int)difftime(time(NULL), start_time), timeout_sec, buffer);
            printf("t%d: Retransmitting Packet %d\n", (int)difftime(time(NULL), start_time), buffer);
        }
    } while (buffer <= size);

    printf("t%d: Transmission completed\n", (int)difftime(time(NULL), start_time));
    printf("Total time required: %d seconds\n", (int)difftime(time(NULL), start_time));

    close(sock);
    return 0;
}
