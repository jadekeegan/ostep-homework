#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

int NUM_ITERATIONS = 1000000;

float measureSystemCallCost(void) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int fd = open("./os-costs.txt", O_RDONLY);
    for (size_t i=0; i < NUM_ITERATIONS; i++) {
        // Perform a simple system call, e.g., read from stdin
        // This is a no-op read to measure the cost of the system call itself
        // without any actual data processing.
      read(0, NULL, 0);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    float timeDiff = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);

    return timeDiff / (float)NUM_ITERATIONS;
}

int main(int argc, char *argv[]) {
  float cost = measureSystemCallCost();
  printf("Avg Sys Call Cost: %f ns\n", cost);
  
  return 0;
}