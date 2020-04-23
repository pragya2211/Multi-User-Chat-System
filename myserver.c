#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define PATH "mypathfile"

struct client
{
    struct sockaddr_un address_client;
    int len,ID_client,val;
};
struct client arr_client[1000];
int n = 0;

void start_socket(int sock)
{
    struct sockaddr_un myserver_addr;
	myserver_addr.sun_family = AF_UNIX;
	strcpy(myserver_addr.sun_path,PATH);
	unlink(PATH);
	int len = strlen(myserver_addr.sun_path)+sizeof(myserver_addr.sun_family);
    if(bind(sock,(struct sockaddr *) &myserver_addr ,len+1) == -1)
    {
        perror("\nCould not bind");
        printf("\nExiting");
        exit(1);
    }
    if(listen(sock,5) == -1)
    {
        perror("listenting error\n");
        printf("\nExiting");
        exit(1);
    }
    printf("Server started------------------------------\n");
    printf("Waiting for clients to join-----------------\n");
}
void slice_string(char *message, char *newmessage,int start,int end)
{
	int j = 0;
	for(int i =start;i<end;i++)
	{
		newmessage[j++] = message[i]; 
	}
	newmessage[j] = 0;
}
void handle_input(int ID,int val)
{
	char message[1000];
    int  reader = recv(ID,message,1000,0);
    message[reader] = '\0';
    if(reader < 0)
    {
        printf("Error in reading!!!\n");
    }
	if(message[0] == '1')
    {
		printf("Message received from Client %d\n",val+1);
		int conv = message[2] - '0';
	 	 printf("Message sent to the client %d\n",conv);
		char newmessage[1001];
		slice_string(message,newmessage,4,strlen(message));
        if(send(arr_client[conv-1].ID_client,newmessage,1000,0) == -1)
        {
            printf("Error: Invalid user entered. Message not sent.....\n");
        }
    }
	else if(message[0] == '2')
    {
		printf("Message received from client %d\n", val+1);
        for(int i = 0; i < n; i++) 
        {
            if(arr_client[i].val != val) 
            {
		         char newmessage[1001];
		         slice_string(message,newmessage,2,strlen(message));
                if(send(arr_client[i].ID_client,newmessage,1000,0) < 0) 
                {
                    printf("Unable to send message \n");
                    continue;
                }
            }
        }
    }
	else if(message[0] == '3')
	{
		printf("The clients available are: \n");
		for(int i =0;i<n;i++)
		{
			printf("Client %d with ID %d\n",arr_client[i].val+1,arr_client[i].ID_client);
		}	
	}
	else if(message[0] == '4')
    {
        printf("Connection broken from Client %d\n", val+1);
        for (int c = val; c < n - 1; c++)
        {
            arr_client[c] = arr_client[c+1]; 
            arr_client[c].val--;
        }
	    n--;
    }
}
void * connect_clients(void *client_socket)
{
    struct client* cl_socket = (struct client*) client_socket;
    long int val = cl_socket -> val;
    long int ID = cl_socket -> ID_client;

    printf("Client connected ------\n");
    printf("Client number: %ld \n",val+1);

    while(1)
    {
        handle_input(ID,val);
	}
}
int main()
{
    pthread_t thread_recv[1000]; 
    int sock;

    sock = socket(AF_UNIX,SOCK_STREAM,0);

    if(sock < 0)
    {
        perror("Socket not created!!");
        exit(1);
    }
    else
    {
        start_socket(sock); //this function is sending the connection request
    }
    int accept_connection = 0;
    while(1)
    {
        accept_connection = accept(sock,(struct sockaddr*)&arr_client[n].address_client, &arr_client[n].len);
        if(accept_connection < 0)
        {
            printf("Unable to establish connection---- accept failed");
        }
        arr_client[n].ID_client = accept_connection;
        arr_client[n].val = n;
        pthread_create(&thread_recv[n], NULL, connect_clients, (void *) &arr_client[n]);
        n ++;
    }  
    return 0;
}

