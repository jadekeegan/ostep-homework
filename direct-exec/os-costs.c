#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

int NUM_ITERATIONS = 1000000;

// Measure the cost of a system call by timing a 0 byte read operation.
// To take the average over many syscalls, time from before we start the reads to after we 
// finish all the reads, and then divide the total time by the number of iterations.
float measureSystemCallCost(void) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int fd = open("./os-costs.txt", O_RDONLY);
    for (size_t i=0; i < NUM_ITERATIONS; i++) {
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