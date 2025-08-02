#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (rc == 0) {
    execlp("/bin/ls", "ls", NULL);
    fprintf(stderr, "Exec failed\n");
    exit(1);
  } else {
    // Parent process waits for child to finish
    wait(NULL);
    printf("Child process finished\n");
  }
}