#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Ensure that child outputs before parent WITHOUT using wait
// This uses a pipe, so the parent must wait for the child to write to the
// before it can output its message.
int main(int argc, char *argv[]) {
  int fd[2];
  if (pipe(fd) == -1) {
    fprintf(stderr, "Pipe creation failed\n");
    exit(1);
  }
  
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("hello from child\n");
    close(fd[0]); // Close read end in child
    write(fd[1], "c", 1); // Write to pipe
    close(fd[1]); // Close write end after writing
  } else {
    close(fd[1]); // Close write end in parent
    char c;
    read(fd[0], &c, 1); // Read from pipe (blocking until data is available)
    close(fd[0]); // Close read end after reading
    printf("goodbye from parent\n");
  }
}