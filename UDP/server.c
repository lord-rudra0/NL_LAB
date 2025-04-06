#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
int main(void) {
int socket_desc;
struct sockaddr_in server_addr, client_addr;
char server_message[2000], client_message[2000];
socklen_t client_struct_length = sizeof(client_addr);
memset(server_message, '\0', sizeof(server_message));
memset(client_message, '\0', sizeof(client_message));
// Create socket
socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
if(socket_desc < 0){
printf("Error while creating socket\n");
return -1;
}
printf("Socket Created Successfully!\n");
// Prepare server address
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(2000);
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
// Bind the socket
if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
printf("Couldn't bind to the port\n");
return -1;
}
printf("Binding Done\n");
// Listening for incoming connections
printf("\nListening for incoming messages!\n");
// Receive message from client
if (recvfrom(socket_desc, client_message, sizeof(client_message), 0, (struct
sockaddr*)&client_addr, &client_struct_length) < 0) {
printf("Couldn't receive\n");
return -1;
}
printf("Message from client: %s\n", client_message);
// Prepare server's response
strcpy(server_message, "This is the server's message.");
// Send response to client
if (sendto(socket_desc, server_message, strlen(server_message), 0, (struct
sockaddr*)&client_addr, client_struct_length) < 0) {
printf("Can't send\n");
return -1;
}
// Close the socket
close(socket_desc);
return 0;
}