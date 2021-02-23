#include "sharedModule.h"




//Functions
string winner(int user, int comp){
	if(user == comp) return "It's a tie";
	else if(user == 1 && comp == 3) return "Player Wins";
	else if(user == 1 && comp == 2) return "Computer Wins";
	else if(user == 2 && comp == 1) return "Player Wins";
	else if(user == 2 && comp == 3) return "Computer Wins";
	else if(user == 3 && comp == 1) return "Computer Wins";
	else if(user == 3 && comp == 2) return "Player Wins";
	return "";
}

string convertNumber(int value){
	switch(value){
		case 1: return "Rock";  break;
		case 2: return "Paper"; break;
		case 3: return "Scissors"; break;
		default: return "default"; break;
	}
}





int main(int argc,char **argv){

	char writeBuffer[1024];
	char readBuf[1024];
	char Cmd[100];
	int toServerG[2];
	int toServerC[2];
	int new_socket;
	int writeError;
	int readErr;
	int valread;
	char sends[500];

	sscanf(argv[0],"%d",&toServerG[0]);
	sscanf(argv[1],"%d",&toServerC[1]);
	sscanf(argv[2],"%d",&new_socket);

  clearBuffer(writeBuffer);
	printf("Child ID: %d\n toServerG :%d\n toServerC :%d\n\n\n", getpid(),toServerG[0], toServerC[1]);
  sprintf(writeBuffer,"ServerG initialized\n\n");
  writeError = write(toServerC[1],writeBuffer,99);

	srand(time(NULL));


	sprintf(sends, "ServerG Initialized");
	send(new_socket , sends , strlen(sends) , 0 );

	valread = read( new_socket , sends, 1024);

//CHOOSE ROCK, PAPER OR SCISSORS

	vector<int> randomValue;
	randomValue = randomChoice();

	int server_Val = 0, user_Val = 0, playerWins = 0, computerWins = 0;
	string who_won;
	string user_s, server_s;

	sprintf(sends, "Game is %d out of %d rounds..\n", randomValue[0], randomValue[1]);
	send(new_socket, sends, strlen(sends), 0);
	valread = read( new_socket , sends, 1024);
	while(computerWins != randomValue[0] && playerWins != randomValue[0])
	{
		server_Val = rand() % 3 + 1;

		memset(sends, 0, sizeof(sends)); //sends nothing just to keep things synced

		sprintf(sends, "Sever val: %d", server_Val);
		send(new_socket, sends, strlen(sends), 0);
		memset(sends, 0, sizeof(sends));
		valread = read(new_socket, sends, 1024);

		user_Val = atoi(sends);
		memset(sends, 0, sizeof(sends));
		who_won = winner(user_Val, server_Val);

		if(who_won == "Player Wins") playerWins++;
		else if(who_won == "Computer Wins") computerWins++;

		who_won = "Computer chose: " + convertNumber(server_Val) +" Player Chose: " + convertNumber(user_Val) +
		"\n" + who_won +"\n"+ "Computer Wins: " + to_string(computerWins) + " Player Wins: " +
		to_string(playerWins) + "\n";


		memset(sends, 0, sizeof(sends));
		strcpy(sends, who_won.c_str());
		send(new_socket, sends, strlen(sends), 0);
		valread = read(new_socket, sends, 1024);

	}

	if(computerWins == randomValue[0]){
		sprintf(sends, "The computer wins the game");
		send(new_socket, sends, strlen(sends), 0);
	}
	else if(playerWins == randomValue[0]){
		sprintf(sends, "The player wins the game");
		send(new_socket, sends, strlen(sends), 0);
	}

	//sprintf(sends, "Exiting...");
	//send(new_socket , sends , strlen(sends) , 0 );

  }
