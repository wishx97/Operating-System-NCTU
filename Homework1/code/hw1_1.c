#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80

int main(void) {
	char *arg[MAX_LINE/2+1];  // command line arguments
	int should_run = 1;  // flag to determine when to exit program
	char inputBuffer[MAX_LINE];
	while(should_run){
		printf("osh>");
		fflush(stdout);
		
		// get input from user
		read(STDIN_FILENO, inputBuffer, MAX_LINE);
		
		// tokenize user input 
		int i = 0;
		char *p = strtok(inputBuffer, " ");
		while (p != NULL) {
			arg[i] = p;
			char *pos;
			if ((pos = strchr(arg[i], '\n')) != NULL) {
				*pos = '\0';
			}
			p = strtok(NULL, " ");
			i++;
		}
		arg[i] = NULL;
		
		// check exit command
		if (strcmp(arg[0], "exit") == 0) {		
			return 0;
		}
		
		// check for background setting
		int background = 0;
		if (strcmp(arg[i - 1], "&") == 0) {
			background = 1;
			arg[i - 1] = NULL;
		}

		// fork a child process with fork
		pid_t pid = fork();
		if (pid < 0) {
			printf("Fork failed.\n");
			exit(1);
		}
		
		// This is child process, which will execute the command
		if (pid == 0) {
			if (execvp(arg[0], arg) == -1) {
				printf("ERROR: Not a valid command.\n");
			}
			exit(1);
		}

		// This is parent process, wait child process here
		else {
			if (!background) {
                waitpid(pid, NULL, 0);
			}	
		}
	}
	return 0;
}

