#include "hal/sdCard.h"

bool mntSuccess = false;

static void runCommand(const char* command)
{
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
    } else {
        mntSuccess = true;
        printf("SD card mounted successfully.\n");
    }
}

// make sure sd card mounted
void mountSDCard() {
	runCommand("sudo mount " SD_CARD_PATH " " MOUNT_PATH);
}

// unmount sd card when done
void unmountSDCard() {
    if (mntSuccess) {
        runCommand("sudo umount " MOUNT_PATH);
        printf("SD card unmounted.\n");
    }
}

bool checkMntSuccess() {
    return mntSuccess;
}

// can cp (or can change to mv) mp4 file
void copyFileToSDCard(const char *source_path) {
    char copyCommand[1024];
    snprintf(copyCommand, sizeof(copyCommand), "sudo cp %s %s", source_path, MOUNT_PATH);
    printf("copyCommand: %s\n", copyCommand);
    runCommand(copyCommand);
}
