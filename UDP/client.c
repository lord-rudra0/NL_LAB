#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    
    // Initialize buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create socket
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_desc < 0) {
        printf("Unable to create socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Get message from user
    printf("Enter message: ");
    fgets(client_message, sizeof(client_message), stdin);

    // Remove newline character from input, if any
    client_message[strcspn(client_message, "\n")] = '\0';

    // Send message to server using UDP
    if (sendto(socket_desc, client_message, strlen(client_message), 0,
               (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to send message\n");
        return -1;
    }

    // Receive response from server
    socklen_t server_struct_length = sizeof(server_addr);
    if (recvfrom(socket_desc, server_message, sizeof(server_message), 0,
                 (struct sockaddr*)&server_addr, &server_struct_length) < 0) {
        printf("Error while receiving server's message\n");
        return -1;
    }

    // Print the server's response
    printf("Server's response: %s\n", server_message);

    // Close the socket
    close(socket_desc);

    return 0;
}
