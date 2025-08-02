#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// waitpid allows the parent to wait for a specific child - useful in cases
// where there are multiple children.
int main(int argc, char *argv[]) {
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("hello from child\n");
  } else {
    int rc_wait = waitpid(rc, NULL, 0); // waits for the child with PID rc
    printf("rc_wait: %d\n", rc_wait);
    printf("goodbye from parent\n");
  }
}