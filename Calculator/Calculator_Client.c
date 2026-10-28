#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

struct Data {
int num1;
int num2;
char op;
};

int main()
{
int clientSocket;
struct sockaddr_in serverAddr;

struct Data d;
int result;
int stop;
socklen_t len = sizeof(serverAddr);

clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(9000);
serverAddr.sin_addr.s_addr = INADDR_ANY;

do {
printf("Enter first number: ");
scanf("%d", &d.num1);

printf("Enter operator (+ - * /): ");
scanf(" %c", &d.op);

printf("Enter second number: ");
scanf("%d", &d.num2);

if(d.op == '/' && d.num2 == 0) 
printf("Can't divide by 0\n");

sendto(clientSocket, &d, sizeof(d), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

recvfrom(clientSocket, &result, sizeof(result), 0, (struct sockaddr *)&serverAddr, &len);

printf("Result = %d\n", result);

printf("Enter 1 to stop else 0: ");
scanf("%d", &stop);

sendto(clientSocket, stop, sizeof(stop), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
} while(stop)

close(clientSocket);

return 0;
}
