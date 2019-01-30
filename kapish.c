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

int main(int argc, char **argv){

	//load config files here
	//.kapishrc
	
	//Buffer to contain user input
	char *buf;

	int keep_running = 1; //keep looping the shell while this is truthy
	while(keep_running){
		printf("? ");
		keep_running = 0;
		read_stdin(&buf);

		free(buf);
	}

	free(buf);
	exit(0);
}

//Take a line of user input
void read_stdin(char** buf){
	//Current position within input string
	int pos = 0;
	*buf = malloc(sizeof(char)*MAX_CHARS);

	if(!*buf){
		fprintf(stderr, "Memory allocation error!\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		//Take a character from stdin
		c = getc(stdin);

		//Handle too-long inputs
		if (pos >= 511){
			fprintf(stderr, "Your input was too long!\n");
			exit(EXIT_FAILURE);
		}
		//If we reach a newline or EOF, replace with \0 and return
		if (c=='\n' || c==EOF){
			*buf[pos] = '\0';
			return;
		}
		else {
			*buf[pos] = c;
		}
		pos++;
	}
}
