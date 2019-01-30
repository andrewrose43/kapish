#include <stdio.h>
#include <stdlib.h>

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
	int keep_running = 1; //keep looping the shell while this is truthy
	while(keep_running){
		printf("yay it compiled");
		keep_running = 0;
	}
	exit(0);
}

