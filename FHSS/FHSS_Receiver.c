
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_WORDS 100
#define MAX_LEN 100

int main() {
    int port[] = {9000, 9001, 9002, 9003, 9004};
    int hop[]  = {3, 1, 0, 4, 2};

    int receiverSocket[5];
    struct sockaddr_in receiverAddress, transmitterAddress;
    socklen_t len;

    char buffer[MAX_LEN];
    char receivedMessage[MAX_WORDS][MAX_LEN];
    int totalHop = 5;

    // Create and bind five UDP sockets
    for (int i = 0; i < 5; i++) {
        receiverSocket[i] = socket(AF_INET, SOCK_DGRAM, 0);

        if (receiverSocket[i] < 0) {
            perror("Socket creation failed");
            return 1;
        }

        int opt = 1;
        setsockopt(receiverSocket[i], SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt));

        memset(&receiverAddress, 0, sizeof(receiverAddress));
        receiverAddress.sin_family = AF_INET;
        receiverAddress.sin_port = htons(port[i]);
        receiverAddress.sin_addr.s_addr = INADDR_ANY;

        if (bind(receiverSocket[i],
                 (struct sockaddr *)&receiverAddress,
                 sizeof(receiverAddress)) < 0) {
            perror("Bind failed");
            return 1;
        }
    }

    printf("FHSS Receiver started. Waiting for words...\n\n");

    // Receive the same number of words as the sender transmits.
    // Enter the expected word count when prompted.
    int totalMsg;
    printf("Enter number of words expected: ");
    scanf("%d", &totalMsg);

    if (totalMsg <= 0 || totalMsg > MAX_WORDS) {
        printf("Invalid number of words.\n");
        for (int i = 0; i < 5; i++)
            close(receiverSocket[i]);
        return 1;
    }

    printf("\nReceiving words:\n");

    for (int i = 0; i < totalMsg; i++) {
        int currentHop = hop[i % totalHop];
        int currentPort = port[currentHop];

        len = sizeof(transmitterAddress);

        int n = recvfrom(receiverSocket[currentHop],
                         buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&transmitterAddress, &len);

        if (n < 0) {
            perror("Receive failed");
            break;
        }

        buffer[n] = '\0';

        strncpy(receivedMessage[i], buffer, MAX_LEN - 1);
        receivedMessage[i][MAX_LEN - 1] = '\0';

        printf("Hop %d | Port %d | Received word: %s\n",
               currentHop, currentPort, buffer);
    }

    printf("\nComplete received message: ");

    for (int i = 0; i < totalMsg; i++)
        printf("%s%s", receivedMessage[i],
               (i == totalMsg - 1) ? "\n" : " ");

    for (int i = 0; i < 5; i++)
        close(receiverSocket[i]);

    return 0;
}
