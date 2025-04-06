#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
int socket_desc;
struct sockaddr_in server_addr;
char buffer[1024];
//for snw
int k=5,m=1,p;
//for snw
// Clean buffers:
//memset(server_message,'\0',sizeof(server_message));
//memset(client_message,'\0',sizeof(client_message));
// Create socket:
socket_desc = socket(AF_INET, SOCK_STREAM, 0);
if(socket_desc < 0){
printf("Unable to create socket\n");
return -1;
}
printf("Socket created successfully\n");
// Set port and IP the same as server-side:
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(2000);
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
// Send connection request to server:
if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) <
0){
printf("Unable to connect\n");
return -1;
}
printf("Connected with server successfully\n");
while(k!=0)
{
if(m<=5)
printf("Sending frame %d\n",m);
if(m%2==0)
strcpy(buffer,"frame");
else
{
strcpy(buffer,"error");
printf("Packet loss\n");
for(p=1;p<=3;p++)
{
printf("Waiting for %d seconds\n",p);
}
printf("retransmitting...\n");
strcpy(buffer,"frame");
sleep(3);
}
int y= send(socket_desc,buffer,19,0);if(y==-1)
{
printf("Error in sending");
exit(1);
}
else
{
printf("Sent frame %d\n",m);
}
int z=recv(socket_desc,buffer,1024,0);
if(z==-1)
printf("Error in receiving data");
if(strncmp(buffer,"ack",3)==0)
printf("Received ACK for frame %d \n",m);
k--;
m++;
}
// Close the socket:
close(socket_desc);
return 0;
}