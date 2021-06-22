#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LEN 100 /* The maximum length command */

int main(void) {
  char *args[MAX_LEN / 2 + 1]; /* command line arguments */
  int should_run = 1;          /* flag to determine when to exit program */
  int background = 0;
  char *command, *s;
  int argv;
  static const char delim[] = " \n";
  pid_t pid;
  int status;

  while (should_run) {
    printf("my_shell> ");
    fflush(stdout);
    /**
     * 표준입출력으로부터 문자열을 입력 받은 후:
     * (1) fork()를 통해 자식 프로세스를 생성
     * (2) 자식 프로세스가 execvp()를 호출하도록 할 것
     * (3) 만약 입력받은 문자에 &가 포함되어 있으면,
     *     부모 프로세스는 wait() 호출하지 않음
     */
    command = (char *)malloc(MAX_LEN * sizeof(char));
    fgets(command, MAX_LEN, stdin);
    argv = 0;
    s = strtok(command, delim);
    if (s == NULL) {
	    continue;
    }
    while (s) {
	    args[argv++] = s;
	    s = strtok(NULL, delim);
    }
    args[argv] = NULL;

    if (strcmp(args[0], "exit") == 0) {
	    free(command);
	    break;
    }

    if (strcmp(args[argv - 1], "&") == 0) {
	    background = 1;
	    args[argv - 1] = NULL;
    }

    pid = fork();
    if (pid < 0) {
	    fprintf(stderr, "fork failed");
	    exit(0);
    } else if (pid == 0) {
	    if (background == 0) {
		    printf("waiting for a child, not a background process\n");
	    } else if (background == 1) {
		    printf("background process\n");
	    }
	    execvp(args[0], args);
	    exit(0);
    } else {
	    if (background == 0) {
		    pid = waitpid(pid, &status, 0);
		    printf("child process complete\n");
	    } else {
		    background = 0;
	    }
    }
  }
  return 0;
}
