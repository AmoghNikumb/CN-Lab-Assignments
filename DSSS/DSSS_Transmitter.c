
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

void decimalToBinary(int n, int binary[], int *size) {
    *size = 0;

    if (n == 0) {
        binary[(*size)++] = 0;
        return;
    }

    while (n > 0) {
        binary[(*size)++] = n % 2;
        n /= 2;
    }

    // Reverse the binary digits
    for (int i = 0; i < *size / 2; i++) {
        int temp = binary[i];
        binary[i] = binary[*size - 1 - i];
        binary[*size - 1 - i] = temp;
    }
}

int main() {
    int data;

    printf("Enter decimal data (0 to 255): ");
    if (scanf("%d", &data) != 1 || data < 0 || data > 255) {
        printf("Invalid input. Enter a number from 0 to 255.\n");
        return 1;
    }

    // 11-bit Barker code
    int barker[11] = {1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0};

    int binary[32];
    int binarySize;

    decimalToBinary(data, binary, &binarySize);

    int spreadData[400];
    int k = 0;

    // Spread each data bit using the Barker code
    for (int i = 0; i < binarySize; i++) {
        for (int j = 0; j < 11; j++) {
            if (binary[i] == 1)
                spreadData[k++] = barker[j];
            else
                spreadData[k++] = !barker[j];
        }
    }

    int transmitterSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (transmitterSocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in receiverAddress;
    memset(&receiverAddress, 0, sizeof(receiverAddress));

    receiverAddress.sin_family = AF_INET;
    receiverAddress.sin_port = htons(9000);
    receiverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send number of chips followed by the spread data
    int packet[401];
    packet[0] = k;
    memcpy(&packet[1], spreadData, k * sizeof(int));

    if (sendto(transmitterSocket, packet,
               (k + 1) * sizeof(int), 0,
               (struct sockaddr *)&receiverAddress,
               sizeof(receiverAddress)) < 0) {
        perror("Send failed");
        close(transmitterSocket);
        return 1;
    }

    printf("\nOriginal Data: %d\n", data);

    printf("Binary: ");
    for (int i = 0; i < binarySize; i++)
        printf("%d", binary[i]);

    printf("\nSpread Data: ");
    for (int i = 0; i < k; i++)
        printf("%d", spreadData[i]);

    printf("\nTotal chips sent: %d\n", k);

    close(transmitterSocket);
    return 0;
}
