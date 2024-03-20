#include "cameraControl.h"
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

static pthread_t recording_tid;
static void* recordingThread(void*);
//static pthread_t conversion_tid;

static int vidIdx = 0;
static int deleteIdx = 5;

static const char* recordCmdFormat = "./capture -F -c 100 -o > ./videos/output%d.raw";
static const char* convertCmdFormat = "ffmpeg -f mjpeg -i ./videos/output%d.raw -vcodec copy ./videos/%d.mp4";

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
    }
}

void CameraControl_init() {
    pthread_create(&recording_tid, NULL, recordingThread, NULL);
}

void CameraControl_cleanup() {
    pthread_join(recording_tid, NULL);
}

void incrementVideo() {
    // keep the range within 10
    vidIdx = (vidIdx + 1) % 11;
    deleteIdx = (deleteIdx + 1) % 11;
}

static void* recordingThread(void*) {
    char recordCmd[1000];
    char convertCmd[1000];

    while(true) {
        sprintf(recordCmd, recordCmdFormat, vidIdx);
        sprintf(convertCmd, convertCmdFormat, vidIdx, vidIdx);
        runCommand(recordCmd);
        runCommand(convertCmd);
        incrementVideo();
    }
}