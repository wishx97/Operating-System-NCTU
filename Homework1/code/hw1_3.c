#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int status;
	printf("Main process id : %d.\n", getpid());
	pid_t pid;
	pid = fork(); // fork 1
	if (pid < 0) {
		printf("Fork 1 Failed.\n");
		exit(1);
	}
	else if (pid == 0) {
		printf("Fork1, I'm the child %d, my parent is %d.\n", getpid(), getppid());
		pid = fork(); // fork 2
		if (pid < 0) {
			printf("Fork 2 Failed.\n");
			exit(1);
		}
		else if (pid == 0){
			printf("Fork2, I'm the child %d, my parent is %d.\n",getpid(),getppid());
		}	
		else {
			wait(NULL);
		}
	}
	else {
		wait(NULL);
	}
	pid = fork(); // fork 3
	if (pid < 0) {
			printf("Fork 3 Failed.\n");
			exit(1);
		}
	else if (pid == 0) {
		printf("Fork3, I'm the child %d, my parent is %d.\n",getpid(),getppid());
	}
	else {
		wait(NULL);
	}
	return 0;
}
