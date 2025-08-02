#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd[2];
  if (pipe(fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  int c1 = fork();
  if (c1 < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (c1 == 0) {
    close(fd[0]); // Close read end in child 1
    write(fd[1], "c", 1); // Write to pipe
    close(fd[1]); // Close write end after writing
    printf("Child 1 wrote to pipe\n");
    exit(EXIT_SUCCESS);
  } else {
    int c2 = fork();
    if (c2 < 0) {
      perror("fork");
      exit(EXIT_FAILURE);
    } else if (c2 == 0) {
      close(fd[1]); // Close write end in child 2
      char c;
      read(fd[0], &c, 1); // Read from pipe
      printf("Child 2 read: %c\n", c); // Print what it read
      close(fd[0]); // Close read end after reading
      exit(EXIT_SUCCESS);
    } else {
      waitpid(c1, NULL, 0); // Wait for child 1
      waitpid(c2, NULL, 0); // Wait for child 2
    }

    printf("Parent finished\n");
  }
}