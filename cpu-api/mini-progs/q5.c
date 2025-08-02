#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "Fork failed\n");
    exit(1);
  } else if (rc == 0) {
    int rc_wait = wait(NULL); // immediately returns -1 bc no children!
    printf("rc_wait: %d\n", rc_wait); // continues execution
    printf("hello from child\n");
  } else {
    int rc_wait = wait(NULL); // returns the child's PID
    printf("rc_wait: %d\n", rc_wait);
    printf("goodbye from parent\n");
  }
}