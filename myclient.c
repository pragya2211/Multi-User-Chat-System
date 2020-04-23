#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "mypathfile\0"

void * receiving(int clt_sock)
{
	char inp[1024];
	int reader = recv(clt_sock,inp,1024,0);
	if(reader < 0)
	{
		printf("Unable to receive!\n");
	}
	inp[reader] = '\0';
	printf("%s\n",inp);
}
void * recv_information(void * sockID)
{
	int clt_sock = *((int *) sockID);
	while(1)
	{	
		receiving(clt_sock);
	}
}
void * send_information(void * sockID)
{
	int sock = *((int *) sockID);	
	while(1)
	{
		char inp[1000];
		fgets(inp,1000,stdin);
		send(sock,inp,1000,0);
		if(inp[0] == '4')
		{
			printf("Exiting!\n");
			break;
		} 
		if(inp[0] != '1' && inp[0] != '2' && inp[0] != '3' && inp[0] != '4')
		{
			printf("Invalid input entered ----------Try Again!\n");
		}
	}	
}
int main()
{
	int sock = socket(AF_UNIX,SOCK_STREAM,0);
	if(sock == -1)
	{
		perror("Error in connection ......................");
		exit(1);
	}
	struct sockaddr_un myserver_addr;
	
	myserver_addr.sun_family = AF_UNIX;
	strcpy(myserver_addr.sun_path,SOCK_PATH);
	if(connect(sock, (struct sockaddr*) &myserver_addr, sizeof(myserver_addr)) == -1)
	{
		return 0;
	}
	printf("Connection established ..............................\n");
	printf("1) Send a message to other client through client number\n");
	printf("2) Send the message to all the clients(Create a broadcast message)\n");
	printf("3) See available clients\n");
	printf("4) Exit from the client\n");
	pthread_t thread_client;
	pthread_create(&thread_client,NULL,recv_information,(void *) &sock);	
	send_information(&sock);
}
