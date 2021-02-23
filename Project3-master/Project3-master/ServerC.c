// Server side C/C++ program to demonstrate Socket programming
#include "ServerC.h"





void *waitForExit(void* arg){
  pid_t wpid;
  int status;
  while(true){
    wpid = wait(&status);
    if(wpid == -1){
      continue;
    }
    else{
      printf("Child (serverG) %d terminated\n", wpid);
    }
  }

}





int main(int argc, char const *argv[])
{

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";


    //STARTING FORK EXIT pthread
    pthread_t exitThread;
    int t = pthread_create(&exitThread, NULL, &waitForExit, NULL);
      if(t != 0){
        printf("Error in exit thread creation\n");
      }
      else{
        printf("Pthread created\n");
      }





    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
	else {printf("bind\n");}

	while(true){
		if (listen(server_fd, 3) < 0)
		{
			perror("listen");
			exit(EXIT_FAILURE);
		}
		else {printf("listening\n");}
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
						   (socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		else {printf("\n\n\n\n-- NEW CONNECTION -- \n\n");}

		//valread = read( new_socket , buffer, 1024);
		//printf("%s\n",buffer );
		//send(new_socket , hello , strlen(hello) , 0 );
		//printf("Hello message sent\n");

    //CREATING SERVERG
    int DBreturn = -1;
  	int err;
  	int Status=0;
  	int writeError, readErr;
  	int toServerG[2];
  	int toServerC[2];
  	char arg1[50];
  	char arg2[50];
    char arg3[50];

  	char readBuf[BUFFER_SIZE+1];
  	char WriteBuffer[BUFFER_SIZE+1];

  	int ServerG_pid;
  	int errPipe;

  	errPipe = pipe(toServerG);

  	if(errPipe == -1){
  			printf("There was an error on pipe creation. Error Number: %d\n", errno);
  			exit(-1);
  		}
  	errPipe = pipe(toServerC);

  	if(errPipe == -1){
  			printf("Error on pipe. Error: %d\n", errno);
  			exit(-1);
  		}

  	ServerG_pid = fork();

  	if(ServerG_pid == -1){
  		printf("Error on fork. Error: %d\n", errno);
  		exit(-1);
  	}

  	else if (ServerG_pid == 0){

  		close(toServerG[1]);
  		close(toServerC[0]);

  		sprintf(arg1,"%d", toServerG[0]);
  		sprintf(arg2,"%d", toServerC[1]);
      sprintf(arg3,"%d", new_socket);
  		err = execl("./serverG",arg1,arg2,arg3,(char *)NULL);

  		if(err == -1){
  			printf("Issue starting ServerG \n");
  			printf("Error :%d", errno);
  			exit(-1);
  		}

  	}else{

  		close(toServerG[0]);
  		close(toServerC[1]);

  		printf("Reading from ServerG\n\n");
  		readErr = read(toServerC[0],readBuf, 99);
  		printf("Response: %s", readBuf);
  		clearBuffer(readBuf);
  		errcheck(readErr);
    }



		close(new_socket);
	}

    return 0;
}
