#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#define PORT 2000
#define BUFFER_SIZE 1024
int main(void)
{
FILE *fp;
char name[100],fileread[100],filename[100],ch,file[100],rcv[100];
int n;
int socket_desc, client_sock, client_size;
struct sockaddr_in server_addr, client_addr;
// Create socket:
socket_desc = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket
if (socket_desc < 0)
{
printf("Error while creating socket\n");
return -1;
}
printf("Socket created successfully\n");
// Set port and IP:
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(2000);
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
// Bind to the set port and IP:
if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
{
printf("Couldn't bind to the port\n");
return -1;
}
printf("Done with binding\n");
// Listen for clients:
if (listen(socket_desc, 1) < 0)
{
printf("Error while listening\n");
return -1;
}
printf("\nListening for incoming connections....\n");
// Accept an incoming connection:
client_size = sizeof(client_addr);
client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
if (client_sock < 0)
{
printf("Can't accept\n");
return -1;
}
printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
printf("Connection accepted\n");
n=recv(client_sock,rcv,100,0);
rcv[n]='\0';
fp=fopen(rcv,"r");
if(fp==NULL)
{
send(client_sock,"error",5,0);
close(client_sock);
}
else
{
while(fgets(fileread,sizeof(fileread),fp))
{
if(send(client_sock,fileread,sizeof(fileread),0)<0)
{
printf("Can't send file contents\n");
}
sleep(1);}
if(!fgets(fileread,sizeof(fileread),fp))
{
printf("Done..\n");
send(client_sock,"completed",9,0);
}
}
// Closing the socket:
fclose(fp);
close(client_sock);
close(socket_desc);
return 0; // Exit the program successfully
}