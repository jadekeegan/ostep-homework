#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int x = 100;
  int rc = fork();
  if (rc < 0) {
    // fork failed; exit
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (rc == 0) {
    x = 50;
    fprintf(stdout, "Child process: x = %d\n", x);
  } else {
    // parent process
    x = 30;
    fprintf(stdout, "Parent process: x = %d\n", x);
    wait(NULL);
  }

}