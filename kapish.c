#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int setenv(const char *var_name, const char *new_value, int change_flag);
int unsetenv(const char *var_name);

#define MAX_CHARS 512
#define WHITESPACES " \t\r\n\a"
#define CONFIG_SCRIPT_NAME ".kapishrc"

int read_stdin(char** buf);
void split_args(char*** destination, char** buf);
int kachow(char** args);
int launch(char** args);

//Take a line of user input
int read_stdin(char** buf){
	printf("ok so far\n");
	
	//Allocate memory for input string
	*buf = malloc(sizeof(char)*MAX_CHARS);
	//Make sure malloc worked
	if(!*buf){
		fprintf(stderr, "Memory allocation error!\n");
		exit(EXIT_FAILURE);
	}

	//Current position within input string
	int pos = 0;

	//Storage for input characters
	char c;

	while(1){
		//Take a character from stdin
		c = getchar();

		//Check for Ctrl-D
		if (c==EOF && pos==0){
			return 0;
		}
		//Handle too-long inputs
		//note: this was not required by the assignment
		if (pos >= MAX_CHARS-1){
			fprintf(stderr, "Your input was too long!\n");
			exit(EXIT_FAILURE);
		}
		//If we reach a newline or EOF, replace with \0 and return
		if (c=='\n' || (c==EOF && pos!=0)){
			(*buf)[pos] = '\0';
			return 1;
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

//Showtime
int kachow(char** args){

	//if no arguments provided, continue
	if (args[0]==NULL) return 1;
		
	//Built-in functions
	if (strcmp(args[0], "setenv")==0){
		if (args[1]==NULL){
	printf("recognized args[1]==NULL correctly\n");
			perror("No environment variable specified");
		}	
		else if (args[2]==NULL){
			setenv(args[1], NULL, 9001);
		}
		else {
			setenv(args[1], args[2], 9001);
		}
		return 1;
	}
	else if (strcmp(args[0], "unsetenv")==0){
		if (args[1]==NULL){
	printf("recognized args[1]==NULL correctly\n");
			perror("No environment variable specified");
		}	
		else {
			printf("args[1] is not null\n");
			unsetenv(args[1]);
		}
		return 1;
	}
	else if (strcmp(args[0], "cd")==0){
		if (args[1] == NULL){
			if (chdir(getenv("HOME"))!=0){
				perror("Error changing directory");
			}
		}	
		else{
			if (chdir(args[1])!=0){
				perror("Error changing directory");
			}
		}
		return 1;
	}
	else if (strcmp(args[0], "exit")==0){
		return 0;
	}
	//Executable
	return launch(args);
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

	//Buffer to contain user input
	char *buf;
	//Argument storage
	char **args;

	//.kapishrc loop goes here
	//open .kapishrc
	FILE *config_script = fopen(strncat(getenv("HOME"), CONFIG_SCRIPT_NAME, 9), "r");
	//ensure file opened correctly
	if (config_script==NULL){
		fprintf(stderr, "unable to open %s", CONFIG_SCRIPT_NAME);
		exit(1);
	}

	//Allocate memory for input string
	*buf = malloc(sizeof(char)*MAX_CHARS);

	while (fgets(buf, MAX_CHARS, config_script)){
		printf("? %s", buf);
		split_args(&args, &buf);
		//execute
		kachow(args);
	}
	free(buf);
	free(args);

	int keep_running = 1; //keep looping the shell while this is truthy

	while(keep_running){

		printf("? ");
		if (!read_stdin(&buf)){
			free(buf);
			break;
		}
		split_args(&args, &buf);

		//EXECUTE
		keep_running = kachow(args);

		free(buf);
		free(args);
	}
	exit(0);
}
