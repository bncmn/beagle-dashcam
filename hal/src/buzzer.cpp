#include "hal/buzzer.h"

static void runCommand(const char* command) {
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

static void writeToFile(const char *pWriteValue, const char *filePath) {
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

void Buzzer_init() {
  runCommand("config-pin P9_22 pwm");
  writeToFile("0", BUZZER_DUTY_CYCLE);
}

void Buzzer_playSound() {
  writeToFile("3033722", BUZZER_PERIOD);
  writeToFile("1516861", BUZZER_DUTY_CYCLE);
  writeToFile("1", BUZZER_ENABLE);
  sleepForMs(100);
  writeToFile("0", BUZZER_DUTY_CYCLE);
  writeToFile("0", BUZZER_ENABLE);
  sleepForMs(50);
  writeToFile("1012384", BUZZER_PERIOD);
  writeToFile("506192", BUZZER_DUTY_CYCLE);
  writeToFile("1", BUZZER_ENABLE);
  sleepForMs(200);
  writeToFile("0", BUZZER_DUTY_CYCLE);
  writeToFile("0", BUZZER_ENABLE);
}

