#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
struct timeval timeout;
void func(int sockfd,int nf,int ws)
{
	char buff[MAX];
	int ack,i=0,n,k,w1=0,w2= ws-1,j,flag=0,count_ack=0;
	if(setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char *)&timeout,sizeof(timeout))<0)
	{
		perror("setsockopt(SO_RECVTIMEO) failed");
	}
	for(i=0;i<nf && i<=w2;i++)
	{
		bzero(buff,sizeof(buff));
		snprintf(buff,sizeof(buff),"%d",i);
		k=send(sockfd,buff,sizeof(buff),0);
		printf("Frame %d sent\n",i);
	}
	while(1)
	{
		if(count_ack==nf)
		{
			strcpy(buff,"Exit");
			k=send(sockfd,buff,sizeof(buff),0);
			break;
		}
		if(w2-w1 != ws-1 && flag==0 && i!=nf)
		{
			bzero(buff,sizeof(buff));
			snprintf(buff,sizeof(buff),"%d",i);
			k=send(sockfd,buff,sizeof(buff),0);
			printf("Frame %d sent\n",i);
			w2++;
			i++;
		}
		flag=0;
		bzero(buff,sizeof(buff));
		n=recv(sockfd,buff,MAX,0);
		ack=atoi(buff);
		if(n>0)
		{
			if(ack+1 == nf)
			{
				printf("Acknowledgement received: %d\nExit\n",ack);
				count_ack++;
				bzero(buff,sizeof(buff));
				/*strcpy(buff,"Exit");
				k=send(sockfd,buff,sizeof(buff),0);
				break;*/
			}
			else if(ack==-1)
			{
				printf("Acknowledgement not received for %d\nResending frame\n",w1);
				bzero(buff,sizeof(buff));
				snprintf(buff,sizeof(buff),"%d",w1);
				k=send(sockfd,buff,sizeof(buff),0);
				printf("Frame %d sent\n",w1);
			}
			else
			{
				w1++;
				printf("Acknowledgement received: %d\n",ack);
				count_ack++;
			}
		}
	}
}
void main()
{
	int sockfd,connfd,f,w;
	struct sockaddr_in servaddr,cli;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		printf("Socket creation failed\n");
		exit(0);
	}
	else
	{
		printf("Socket successfully created\n");
		bzero(&servaddr,sizeof(servaddr));
		servaddr.sin_family=AF_INET;
		servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
		servaddr.sin_port=htons(PORT);
		timeout.tv_sec=3;
		timeout.tv_usec=0;
		if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr)) != 0)
		{
			printf("Connection with the server failed\n");
			exit(0);
		}
		else
		{
			printf("Connected to the server\n");
			printf("Enter the number of frames: ");
			scanf("%d", &f);
			printf("Enter the window size: ");
			scanf("%d", &w);
			func(sockfd,f,w);
			close(sockfd);
		}
	}
}