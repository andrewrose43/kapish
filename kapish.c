#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 512
#define WHITESPACES " \t\r\n\a"

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

void split_args(char*** destination, char** buf){
	
	//the argument dump location
	*destination = malloc(sizeof(char*) * MAX_CHARS);
	//individual token temporary storage
	char *tok;

	//Current position within input string
	int pos = 0;

	if (!*destination){
		fprintf(stderr, "Memory allocation error!\n");
		exit(EXIT_FAILURE);
	}
	
	tok = strtok(*buf, WHITESPACES);

	while (tok!=NULL){
		(*destination)[pos++] = tok;
		
		if (pos >= MAX_CHARS){
			fprintf(stderr, "String too long passed to split_args somehow!\n");
			exit(EXIT_FAILURE);
		}
		//Next token
		tok = strtok(NULL, WHITESPACES);
	}
}

int launch(char **args){
	int status;
	int pid = fork();

	if (pid<0){
		//fork failed
		//print error message and keep going
		perror("Fork failed");
	}
	else if (pid==0){
		//CHILD
		if (execvp(args[0], args) == -1) { //this is where it switches to the new process
			perror("Fork failed");
		}
		exit(EXIT_FAILURE); //exit_failure if execvp did not change processes
	}
	else{
		//in this case, it's a parent process
		do{
			//wait for the child to finish
			waitpid(pid, &status, WUNTRACED);
		//while the child has not finished
		}while (!WIFEXITED(status));
	}
	return 1;
}

int main(int argc, char **argv){

	//load config files here
	//.kapishrc
	
	//Buffer to contain user input
	char *buf;
	//Argument storage
	char **args;

	int keep_running = 1; //keep looping the shell while this is truthy
	while(keep_running){

		//Allocate memory for input string
		buf = malloc(sizeof(char)*MAX_CHARS);
		//Make sure malloc worked
		if(!buf){
			fprintf(stderr, "Memory allocation error!\n");
			exit(EXIT_FAILURE);
		}

		printf("? ");
		read_stdin(&buf);
		split_args(&args, &buf);

		//EXECUTE
		kachow(args);

		printf("\n%s\n", args[0]);

		free(buf);
		free(args);

		//REMOVE
		keep_running = 0;
	}

	//free(buf);
	exit(0);
}
