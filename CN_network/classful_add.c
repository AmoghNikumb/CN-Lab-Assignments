
#include <stdio.h>
#include <math.h>

#define MAX 100

int main()
{
    int n, i;
    int a[MAX], b[MAX], c[MAX], d[MAX];

    char ip[MAX][20];
    char className[MAX][10];
    char lba[MAX][20];
    char dba[MAX][20];

    unsigned long long noOfNW[MAX];
    unsigned long long noOfHost[MAX];

    int networkBits, hostBits;

    printf("Enter number of IP addresses: ");

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
    {
        printf("Invalid number of IP addresses.\n");
        return 1;
    }

    printf("\nEnter %d IP addresses:\n\n", n);

    // Input all IP addresses
    for (i = 0; i < n; i++)
    {
        printf("IP %d: ", i + 1);

        if (scanf("%d.%d.%d.%d",
                  &a[i], &b[i], &c[i], &d[i]) != 4)
        {
            printf("Invalid IP address format.\n");
            return 1;
        }

        // Validate each octet
        if (a[i] < 0 || a[i] > 255 ||
            b[i] < 0 || b[i] > 255 ||
            c[i] < 0 || c[i] > 255 ||
            d[i] < 0 || d[i] > 255)
        {
            printf("Invalid IP address.\n");
            return 1;
        }

        // Store the IP address
        snprintf(ip[i], sizeof(ip[i]), "%d.%d.%d.%d",
                 a[i], b[i], c[i], d[i]);
    }

    // Process each IP address
    for (i = 0; i < n; i++)
    {
        // Default values
        networkBits = 0;
        hostBits = 0;

        snprintf(lba[i], sizeof(lba[i]), "255.255.255.255");

        // Class A
        if (a[i] >= 1 && a[i] <= 126)
        {
            snprintf(className[i], sizeof(className[i]), "A");

            networkBits = 7;
            hostBits = 24;

            snprintf(dba[i], sizeof(dba[i]),
                     "%d.255.255.255", a[i]);
        }

        // Class B
        else if (a[i] >= 128 && a[i] <= 191)
        {
            snprintf(className[i], sizeof(className[i]), "B");

            networkBits = 14;
            hostBits = 16;

            snprintf(dba[i], sizeof(dba[i]),
                     "%d.%d.255.255", a[i], b[i]);
        }

        // Class C
        else if (a[i] >= 192 && a[i] <= 223)
        {
            snprintf(className[i], sizeof(className[i]), "C");

            networkBits = 21;
            hostBits = 8;

            snprintf(dba[i], sizeof(dba[i]),
                     "%d.%d.%d.255", a[i], b[i], c[i]);

        }

        // Class D
        else if (a[i] >= 224 && a[i] <= 239)
        {
            snprintf(className[i], sizeof(className[i]), "D");
            noOfNW[i] = 0;
            noOfHost[i] = 0;

            snprintf(dba[i], sizeof(dba[i]), "N/A");
            snprintf(lba[i], sizeof(lba[i]), "N/A");
            continue;
        }

        // Class E
        else if (a[i] >= 240 && a[i] <= 255)
        {
            snprintf(className[i], sizeof(className[i]), "E");
            noOfNW[i] = 0;
            noOfHost[i] = 0;

            snprintf(dba[i], sizeof(dba[i]), "N/A");
            snprintf(lba[i], sizeof(lba[i]), "N/A");
            continue;
        }

        // Reserved first octets: 0 and 127
        else
        {
            snprintf(className[i], sizeof(className[i]), "Special");
            noOfNW[i] = 0;
            noOfHost[i] = 0;

            snprintf(lba[i], sizeof(lba[i]), "N/A");
            snprintf(dba[i], sizeof(dba[i]), "N/A");
            continue;
        }

        // Calculate number of networks and hosts using formulas
        noOfNW[i] = (1ULL << networkBits);
        noOfHost[i] = (1ULL << hostBits) - 2;

        // Class A excludes reserved network numbers 0 and 127
        if (a[i] >= 1 && a[i] <= 126)
        {
            noOfNW[i] -= 2;
        }
    }

    // Display results
    for (i = 0; i < n; i++)
    {
        printf("\n IP ADDRESS %d \n", i + 1);

        printf("IP Address               : %s\n", ip[i]);
        printf("Class                    : %s\n", className[i]);

        if (className[i][0] == 'A' ||
            className[i][0] == 'B' ||
            className[i][0] == 'C')
        {
            printf("Number of Networks       : %llu\n", noOfNW[i]);
            printf("Number of Hosts          : %llu\n", noOfHost[i]);
        }
        else
        {
            printf("Number of Networks       : N/A\n");
            printf("Number of Hosts          : N/A\n");
        }

        printf("Limited Broadcast (LBA)  : %s\n", lba[i]);
        printf("Directed Broadcast (DBA) : %s\n", dba[i]);
    }

    return 0;
}
