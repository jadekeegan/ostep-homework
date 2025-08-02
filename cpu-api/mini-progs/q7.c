#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// If the child closes standard output, it can no longer print.
// The standard output for the parent will still work though as it processes
// have their own stdout channels, so it can print to the terminal even
// after the child closes its standard output.
int main(int argc, char *argv[]) {
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("c stdout_file descriptor: %d\n", STDOUT_FILENO);
    close(STDOUT_FILENO); // Close standard output
    printf("hello from child\n"); // This will not print anything
  } else {
    wait(NULL);
    printf("p stdout_file descriptor: %d\n", STDOUT_FILENO);
    printf("goodbye from parent\n");
  }
}