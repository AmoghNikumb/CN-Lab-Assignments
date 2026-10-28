
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

    int transmitterSocket[5];
    struct sockaddr_in receiverAddress[5];

    char message[MAX_LEN];
    char words[MAX_WORDS][MAX_LEN];
    int totalMsg = 0;

    // Create five UDP sockets and configure receiver addresses
    for (int i = 0; i < 5; i++) {
        transmitterSocket[i] = socket(AF_INET, SOCK_DGRAM, 0);

        if (transmitterSocket[i] < 0) {
            perror("Socket creation failed");
            return 1;
        }

        memset(&receiverAddress[i], 0, sizeof(receiverAddress[i]));
        receiverAddress[i].sin_family = AF_INET;
        receiverAddress[i].sin_port = htons(port[i]);
        receiverAddress[i].sin_addr.s_addr = inet_addr("127.0.0.1");
    }

    // Take message from user
    printf("Enter the message to transmit: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    // Split message into words
    char *token = strtok(message, " ");

    while (token != NULL && totalMsg < MAX_WORDS) {
        strncpy(words[totalMsg], token, MAX_LEN - 1);
        words[totalMsg][MAX_LEN - 1] = '\0';
        totalMsg++;
        token = strtok(NULL, " ");
    }

    if (totalMsg == 0) {
        printf("No message entered.\n");
        for (int i = 0; i < 5; i++)
            close(transmitterSocket[i]);
        return 1;
    }

    printf("\nFHSS Transmission Started\n");
    printf("Hop sequence: 3 -> 1 -> 0 -> 4 -> 2\n\n");

    // Transmit one word per hop
    for (int i = 0; i < totalMsg; i++) {
        int currentHop = hop[i % 5];
        int currentPort = port[currentHop];

        sendto(transmitterSocket[currentHop],
               words[i],
               strlen(words[i]) + 1,
               0,
               (struct sockaddr *)&receiverAddress[currentHop],
               sizeof(receiverAddress[currentHop]));

        printf("Hop %d | Port %d | Sending word: %s\n",
               currentHop, currentPort, words[i]);

        sleep(1);
    }

    printf("\nMessage transmission completed.\n");

    for (int i = 0; i < 5; i++)
        close(transmitterSocket[i]);

    return 0;
}
