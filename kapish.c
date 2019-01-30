#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS 512

/*int main(int argc, char **argv){
	while(1){
		printf("$ ");
		char *cmd = get_next_command();
		int pid = fork();
		if (pid==0){
			exec(cmd);
			panic("exec failed!");
		}
		else{
			wait(pid);
		}
	}
}
*/

//Take a line of user input
void read_stdin(char** buf){
	//Current position within input string
	int pos = 0;

	//Storage for input characters
	char c;

	//Allocate memory for input string
	*buf = malloc(sizeof(char)*MAX_CHARS);
	//Make sure malloc worked
	if(!*buf){
		fprintf(stderr, "Memory allocation error!\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		//Take a character from stdin
		c = getchar();

		//Handle too-long inputs
		if (pos >= MAX_CHARS-1){
			fprintf(stderr, "Your input was too long!\n");
			exit(EXIT_FAILURE);
		}
		//If we reach a newline or EOF, replace with \0 and return
		if (c=='\n' || c==EOF){
			(*buf)[pos] = '\0';
			return;
		}
		//else we add a character to the buffer
		else {
			(*buf)[pos] = c;
		}
		pos++;
	}
}

int main(int argc, char **argv){

	//load config files here
	//.kapishrc
	
	//Buffer to contain user input
	char *buf;

	int keep_running = 1; //keep looping the shell while this is truthy
	while(keep_running){
		printf("? ");
		read_stdin(&buf);

		free(buf);

		//REMOVE
		keep_running = 0;
	}

	free(buf);
	exit(0);
}
