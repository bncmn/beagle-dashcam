#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include "tools.h"

void runCommand(char* command) {
  // Execute the shell command (output into pipe)
  FILE *pipe = popen(command, "r");
  
  // Ignore output of the command; but consume it
  // so we don't get an error when closing the pipe.
  char buffer[1024];
  while (!feof(pipe) && !ferror(pipe)) {
    if (fgets(buffer, sizeof(buffer), pipe) == NULL)
    break;
    // printf("--> %s", buffer); // Uncomment for debugging
  }
  
  // Get the exit code from the pipe; non-zero is an error:
  int exitCode = WEXITSTATUS(pclose(pipe));
  if (exitCode != 0) {
    perror("Unable to execute command:");
    printf(" command: %s\n", command);
    printf(" exit code: %d\n", exitCode);
  }
}

void writeToFile(char *pWriteValue, char *filePath) {
  FILE *pFile = fopen(filePath, "w");
  if (pFile == NULL) {
    printf("[ERROR] Unable to open file for write.\n");
    exit(-1);
  }

  int charWritten = fprintf(pFile, pWriteValue);
  if (charWritten <= 0) {
    printf("[ERROR] Error when writing to file.\n");
    exit(-1);
  }

  fclose(pFile);
}

void sleepForMs(long long delayInMs) {
  const long long NS_PER_MS = 1000 * 1000;
  const long long NS_PER_SECOND = 1000000000;
  long long delayNs = delayInMs * NS_PER_MS;
  int seconds = delayNs / NS_PER_SECOND;
  int nanoseconds = delayNs % NS_PER_SECOND;
  struct timespec reqDelay = {seconds, nanoseconds};
  nanosleep(&reqDelay, (struct timespec *) NULL);
}