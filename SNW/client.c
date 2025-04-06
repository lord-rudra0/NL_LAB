#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
int socket_desc, client_sock, client_size;
struct sockaddr_in server_addr, client_addr;
char buffer[1024];
//for snw
int k=5,m=1,p;
//for snw
// Create socket:
socket_desc = socket(AF_INET, SOCK_STREAM, 0);
if(socket_desc < 0){
printf("Error while creating socket\n");
return -1;
}
printf("Socket created successfully\n");
// Set port and IP:
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(2000);
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
// Bind to the set port and IP:
if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
printf("Couldn't bind to the port\n");
return -1;
}
printf("Done with binding\n");
// Listen for clients:
if(listen(socket_desc, 1) < 0){
printf("Error while listening\n");
return -1;
}
printf("\nListening for incoming connections.....\n");
// Accept an incoming connection:
client_size = sizeof(client_addr);
client_sock = accept(socket_desc, (struct sockaddr*)&client_addr,
&client_size);
if (client_sock < 0){
printf("Can't accept\n");
return -1;
}
printf("Client connected at IP: %s and port: %i\n",
inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
while(k!=0)
{
//demo for 5 packets
int y = recv(client_sock,buffer,1024,0);
if(y==-1){
printf("Error in receiving");
exit(1);
}
if(strncmp(buffer,"frame",5)==0)
printf("Received frame %d successfully\n",m);
else
printf("Frame %d not received\n",m);
if(m%2==0)
strcpy(buffer,"ack");
else
{
strcpy(buffer,"kca");
printf("Ack lost\n");
for(p=1;p<=3;p++)
{
printf("Waiting for %d seconds\n",p);
}
printf("Retransmitting ack...\n");
strcpy(buffer,"ack");
sleep(3);
}
printf("sending ack %d\n",m);
int z= send(client_sock, buffer,19,0);
if(z==-1)
{
printf("Error in sending");
exit(1);
}
k--;
m++;
}
// Closing the socket:
close(client_sock);
close(socket_desc);
return 0;
}