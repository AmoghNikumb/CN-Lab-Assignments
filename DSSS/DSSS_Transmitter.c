#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

void decimalToBinary(int n, int binary[], int *size){

*size = 0;

while(n > 0){
binary[*size] = n%2;
(*size)++;
n /= 2;
}

for(int i=0; i<*size/2; i++){
int temp = binary[i];
binary[i] = binary[*size-1-i];
binary[*size-1-i] = temp;
}

}

int main(){

int data = 45;

int barker[11] = {1,0,1,1,0,1,1,1,0,0,0};

int binary[32];
int binarySize;

decimalToBinary(data, binary, &binarySize);

int spreadData[400];
int k = 0;

for(int i=0; i<binarySize; i++){

if(binary[i] == 1){
for(int j=0; j<11; j++)
spreadData[k++] = barker[j];
}
else{
for(int j=0; j<11; j++)
spreadData[k++] =! barker[j];
}
 
}

int transmitterSocket;

struct sockaddr_in receiverAddress;

transmitterSocket = socket(AF_INET,SOCK_DGRAM,0);

receiverAddress.sin_family = AF_INET;
receiverAddress.sin_port = htons(9000);
receiverAddress.sin_addr.s_addr = INADDR_ANY;

sendto(transmitterSocket, spreadData, k*sizeof(int), 0, (struct sockaddr *)&receiverAddress, sizeof(receiverAddress));

printf("Original Data : %d\n",data);

printf("Binary : ");
for(int i=0; i<binarySize; i++)
printf("%d",binary[i]);

printf("\n");

printf("Spread Data : ");
for(int i=0; i<k; i++)
printf("%d",spreadData[i]);

printf("\n");

close(transmitterSocket);

return 0;

}
