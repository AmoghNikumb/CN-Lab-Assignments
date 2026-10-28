#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

struct Data {
int num1;
int num2;
char op;
int stop;
};

int main()
{
int serverSocket;
struct sockaddr_in serverAddr, clientAddr;
socklen_t len = sizeof(clientAddr);

struct Data d;
int result;
int stop;

serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(9000);
serverAddr.sin_addr.s_addr = INADDR_ANY;

bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

printf("Server is waiting...\n");

do {
recvfrom(serverSocket, &d, sizeof(d), 0, (struct sockaddr *)&clientAddr, &len);

switch(d.op) {
case '+':
result = d.num1 + d.num2;
break;

case '-':
result = d.num1 - d.num2;
break;

case '*':
result = d.num1 * d.num2;
break;

case '/':
if(d.num2 == 0) {
printf("Can't divide by 0\n");
result = 0;
}
else {
result = d.num1 / d.num2;
}
break;

default:
result = 0;
}

sendto(serverSocket, &result, sizeof(result), 0, (struct sockaddr *)&clientAddr, len);

printf("Result Sent = %d\n", result);

recvfrom(serverSocket, stop, sizeof(stop), 0, (struct sockaddr *)&clientAddr, &len);
} while (stop)

return 0;
}
