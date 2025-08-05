#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/syscall.h>

int NUM_ITERATIONS = 1000000;

// Measure the cost of a system call by timing a 0 byte read operation.
// To take the average over many syscalls, time from before we start the reads to after we 
// finish all the reads, and then divide the total time by the number of iterations.
float measureSystemCallCost(void) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int fd = open("./os-costs.txt", O_RDONLY);
    for (int i=0; i < NUM_ITERATIONS; i++) {
      read(0, &fd, 0);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    float timeDiff = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);

    return timeDiff / (float)NUM_ITERATIONS;
}

// Measure the cost of a context switch by simulating lmbench.
// We create two processes that continuously "context switch" while they wait
// for the other process to write to the pipe they are attempting to read from.
float measureContextSwitchCost(void) {
  // CPU affinity
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);

  // Pipes
  int fd1[2], fd2[2];
  pipe(fd1);
  pipe(fd2);

  int rc = fork();

  if (rc < 0) {
    perror("Fork failed");
    exit(1);
  } else if (rc == 0) {
    // Child Process: Writes to pipe 2 and reads from pipe 1
    // Ensure processes run on CPU 0
    sched_setaffinity(0, sizeof(cpu_set_t), &mask);

    close(fd1[1]); // Close write end of pipe 1
    close(fd2[0]); // Close read end of pipe 2

    char buffer;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      read(fd1[0], &buffer, 1); // Read from pipe 1
      write(fd2[1], "2", 1); // Write to pipe 2
    }

    close(fd1[0]); // Close read end of pipe 1
    close(fd2[1]); // Close write end of pipe 2
    exit(EXIT_SUCCESS);
  } else {
    // Start timer
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Parent Process: Writes to pipe 1 and reads from pipe 2
    // Ensure processes run on CPU 0
    sched_setaffinity(0, sizeof(cpu_set_t), &mask);

    close(fd1[0]); // Close read end of pipe 1
    close(fd2[1]); // Close write end of pipe 2

    char buffer;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      write(fd1[1], "1", 1); // Write to pipe 1
      read(fd2[0], &buffer, 1); // Read from pipe 2
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    float timeDiff = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);

    // We don't really care about the time taken by child process to close the pipes,
    // so we don't need to wait for the child before the timer ends.
    wait(NULL); // Wait for child process to finish

    close(fd1[1]); // Close write end of pipe 1
    close(fd2[0]); // Close read end of pipe 2

    return timeDiff / (float)NUM_ITERATIONS;
  }
}

// NOTE: Must run with -pthread to ensure proper linking of pthread library
// Example: gcc -o os-costs os-costs.c -pthread
int main(int argc, char *argv[]) {
  float systemCallCost = measureSystemCallCost();
  printf("Avg System Call Cost: %f ns\n", systemCallCost);
  
  float contextSwitchCost = measureContextSwitchCost();
  printf("Avg Context Switch Cost: %f ns\n", contextSwitchCost);

  return 0;
}