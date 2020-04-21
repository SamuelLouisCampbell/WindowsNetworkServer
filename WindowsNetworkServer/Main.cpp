#define PORT 7000
#define IP_ADDRESS 127.0.0.1

#include <algorithm>
#include<io.h>
#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc, char* argv[])
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	const char* message;
	const int maxC_Message = 512; 
	char clientMessage[maxC_Message];
	int recv_size; 

	printf("\nInitialising Winsock... ");
	if (WSAStartup(MAKEWORD(2, 2), & wsa) != 0)
	{
		printf("Failed.Error Code : % d " , WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n ");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : % d ", WSAGetLastError());
	}

	printf("\n>>>Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(s, (struct sockaddr*) & server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : % d ", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	puts(">>>Bind complete\n");

	//Listen to incoming connections
	listen(s, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);
	
	while ((new_socket = accept(s, (struct sockaddr*) & client, & c)) != INVALID_SOCKET)
	{
		puts("\nConnection accepted ");

		if ((recv_size = recv(new_socket, clientMessage, maxC_Message, 0)) == SOCKET_ERROR)
		{
			puts("No messages...\n");
		}
		else
		{
			puts("\nWe got this from the client:");
			if (recv_size > maxC_Message)
			{
				clientMessage[maxC_Message - 1] = '\0';
			}
			else
			{
				clientMessage[recv_size] = '\0';
			}
			puts(clientMessage);
		}

		
		//Reply to the client
		message = "Hello Client, I have received your connection.But I have to go now, bye\n";
		send(new_socket, message, strlen(message), 0);
	}

	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : % d ", WSAGetLastError());
		return 1;
	}



	closesocket(s);
	WSACleanup();

	return 0;
}