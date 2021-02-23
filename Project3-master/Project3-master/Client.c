// Client side C/C++ program to demonstrate Socket programming
#include "sharedModule.h"


int main(int argc, char const *argv[])
{

	vector<int> random;

  //CREATES SOCKET
	int valread;
	int err;
	int sock;
	char buffer[1024] = {0};
	struct sockaddr_in cAddr;
	int cSocLen;
	struct addrinfo criteria;
	struct addrinfo *serverResult;;
	
	// get server address.
	memset (&criteria, 0, sizeof (struct addrinfo));
	criteria.ai_family = AF_INET;
	criteria.ai_socktype = SOCK_STREAM;
	criteria.ai_protocol = 0; //any
	err = getaddrinfo (SERVERNAME, SERVERPORTSTR, &criteria,
	&serverResult);
	if (err != 0) {
		perror ( gai_strerror (err) );
		exit (1);
	}
	// create socket and connect to the slected server.
	sock = socket ( AF_INET, SOCK_STREAM, 0); // AF_INET
	if (sock == -1) {
		perror ("socClient: socket creation failed");
		exit (2);
	}
	err = connect (sock, serverResult->ai_addr,sizeof(struct sockaddr_in));
	if(err == -1){
		printf("Connection Failed..");
		exit(2);
	}
    //END OF SOCKET CREATION



    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );

    char *hello = "Client Initialized";
    send(sock , hello , strlen(hello) , 0 );

	string server_choice;
	string Sclient_choice;
	char client_choice[50];
	valread = read( sock , buffer, 1024);
	printf("%s\n",buffer );
	send(sock , hello , strlen(hello) , 0 );
		while(true){
			memset(buffer, 0, sizeof(buffer));
			valread = read( sock , buffer, 1024);
			if((strcmp(buffer, "The computer wins the game") == 0)
			 || (strcmp(buffer, "The player wins the game") == 0)){
				 break;
			 }
			server_choice = buffer;
			Sclient_choice = "";
		
			cout << "Enter one of the following: \n 1 ~ Rock \n 2 ~ Paper \n 3 ~ Scissors\n:: ";
			cin >> Sclient_choice;
			

			memset(client_choice, 0, sizeof(client_choice));
			strcpy(client_choice, Sclient_choice.c_str());

			send(sock , client_choice , strlen(client_choice) , 0 );
			valread = read( sock , buffer, 1024);
			printf("%s\n",buffer );
			hello = "Continue";
			send(sock , hello , strlen(hello) , 0 );


		}
    printf("%s\n",buffer );

    return 0;
}
