
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int binaryToDecimal(int binary[], int size) {
    int decimal = 0;

    for (int i = 0; i < size; i++)
        decimal = decimal * 2 + binary[i];

    return decimal;
}

int main() {
    int receiverSocket;
    struct sockaddr_in receiverAddress, transmitterAddress;
    socklen_t len = sizeof(transmitterAddress);

    int packet[401];
    int receivedData[400];

    receiverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (receiverSocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&receiverAddress, 0, sizeof(receiverAddress));

    receiverAddress.sin_family = AF_INET;
    receiverAddress.sin_port = htons(9000);
    receiverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(receiverSocket,
             (struct sockaddr *)&receiverAddress,
             sizeof(receiverAddress)) < 0) {
        perror("Bind failed");
        close(receiverSocket);
        return 1;
    }

    printf("Receiver waiting for spread data...\n");

    int bytes = recvfrom(receiverSocket, packet,
                         sizeof(packet), 0,
                         (struct sockaddr *)&transmitterAddress, &len);

    if (bytes < (int)sizeof(int)) {
        printf("Invalid or empty packet received.\n");
        close(receiverSocket);
        return 1;
    }

    int chips = packet[0];

    if (chips <= 0 || chips > 400 ||
        chips % 11 != 0 ||
        bytes != (chips + 1) * (int)sizeof(int)) {
        printf("Invalid chip data received.\n");
        close(receiverSocket);
        return 1;
    }

    memcpy(receivedData, &packet[1], chips * sizeof(int));

    printf("\nReceived Chips: ");
    for (int i = 0; i < chips; i++)
        printf("%d", receivedData[i]);

    int barker[11] = {1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0};

    int binary[32];
    int bitCount = 0;

    // Despread each group of 11 chips
    for (int i = 0; i < chips; i += 11) {
        int ones = 0;
        int zeros = 0;

        for (int j = 0; j < 11; j++) {
            if (receivedData[i + j] == barker[j])
                ones++;
            else
                zeros++;
        }

        // Choose the closest match
        binary[bitCount++] = (ones > zeros) ? 1 : 0;
    }

    printf("\n\nRecovered Binary: ");
    for (int i = 0; i < bitCount; i++)
        printf("%d", binary[i]);

    int original = binaryToDecimal(binary, bitCount);

    printf("\nRecovered Decimal Data: %d\n", original);

    close(receiverSocket);
    return 0;
}
