#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  FILE *file = fopen("q2-sample.txt", "w");
  if (file == NULL) {
    fprintf(stderr, "Failed to open file\n");
    exit(1);
  }

  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "Fork failed\n");
    fclose(file);
    exit(1);
  } else if (rc == 0) {
    fprintf(file, "Child process writing to file\n");
    fclose(file);
  } else {
    fprintf(file, "Parent process writing to file\n");
    fclose(file);
  }
}