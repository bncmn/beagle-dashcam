#include "joystick.h"

const int JOYSTICK_PINS[] = {26, 27, 47, 46, 65};

static void runCommand(char *command) {
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

static void sleepForMs(long long delayInMs) {
  const long long NS_PER_MS = 1000 * 1000;
  const long long NS_PER_SECOND = 1000000000;
  long long delayNs = delayInMs * NS_PER_MS;
  int seconds = delayNs / NS_PER_SECOND;
  int nanoseconds = delayNs % NS_PER_SECOND;
  struct timespec reqDelay = {seconds, nanoseconds};
  nanosleep(&reqDelay, (struct timespec *) NULL);
}

FILE *openJoystickFile(int pin, const char *mode) {
    char filePath[BUFFER_MAX_LENGTH];
    sprintf(filePath, GPIO_BASE_PATH "gpio%d/value", pin);
    FILE *pFile = fopen(filePath, mode);

    if (pFile == NULL) {
        if (*mode == 'w') {
            printf("ERROR: Unable to open export file.\n");
            exit(1);
        }
        else {
            printf("ERROR: Unable to open file (%s) for read\n", filePath);
            exit(-1);
        }
    }

    return pFile;
}

void writeJoystickFile(int pin) {
    FILE *joystickFileWrite = openJoystickFile(pin, "w");

    // Write to data to the file using fprintf():
    fprintf(joystickFileWrite, "in");
    // Close the file using fclose():
    fclose(joystickFileWrite);
    // Call nanosleep() to sleep for ~300ms before use.
    sleepForMs(300);
}

bool readJoystickFile(int pin) {
    FILE *joystickFileRead = openJoystickFile(pin, "r");

    // Read string (line)
    char buff[BUFFER_MAX_LENGTH];
    fgets(buff, BUFFER_MAX_LENGTH, joystickFileRead);
    
    // Close
    fclose(joystickFileRead);
    // printf("Read: '%s'\n", buff);

    return atoi(buff) == 0;
}

bool pressedDown(void) {
    return readJoystickFile(JOYSTICK_PINS[3]);
}

void initJoystickDown() {
    runCommand("config-pin p8.16 gpio");
    writeJoystickFile(JOYSTICK_PINS[3]);
}
