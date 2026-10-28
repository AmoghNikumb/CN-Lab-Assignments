#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

int binaryToDecimal(int binary[], int size) {

int decimal = 0;

for(int i=0; i<size; i++)
decimal = decimal*2 + binary[i];

return decimal;

}

void despread(int receivedData[], int chips){

int barker[11]={1,0,1,1,0,1,1,1,0,0,0};

int binary[32];
int bitCount = 0;

for(int i=0; i<chips; i+=11){

int match = 1;

for(int j=0; j<11; j++){

if(receivedData[i+j] != barker[j]){
match = 0;
break;
}

}

if(match)
binary[bitCount++] = 1;
else
binary[bitCount++] = 0;

}

printf("\nRecovered Binary : ");

for(int i=0; i<bitCount; i++)
printf("%d",binary[i]);

printf("\n");

int original = binaryToDecimal(binary, bitCount);

printf("Original Data : %d", original);

}

int main(){

int receiverSocket;

struct sockaddr_in receiverAddress, transmitterAddress;

socklen_t len = sizeof(transmitterAddress);

int receivedData[400];

receiverSocket=socket(AF_INET,SOCK_DGRAM,0);

receiverAddress.sin_family = AF_INET;
receiverAddress.sin_port = htons(9000);
receiverAddress.sin_addr.s_addr = INADDR_ANY;

bind(receiverSocket, (struct sockaddr *)&receiverAddress, sizeof(receiverAddress));

int bytes = recvfrom(receiverSocket, receivedData, sizeof(receivedData), 0, (struct sockaddr *)&transmitterAddress, &len);

int chips = bytes/sizeof(int);

printf("Received Chips : ");

for(int i=0; i<chips; i++)
printf("%d", receivedData[i]);

despread(receivedData, chips);

printf("\n");

close(receiverSocket);

return 0;

}
