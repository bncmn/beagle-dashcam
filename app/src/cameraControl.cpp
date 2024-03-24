#include "cameraControl.h"
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "hal/cameraTrigger.h"
#include <time.h>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

#define MAX_STR_LEN 100

static pthread_t recording_tid;
static void* recordingThread(void*);
static pthread_t conversion_tid;
static void* conversionThread(void*);

static int vidIdx = 0;
static int deleteIdx = 5;

CameraEvent motionEvent;

static const char* recordCmdFormat = "./capture -F -c 100 -o > ./videos/output%d.raw";
static const char* convertCmdFormat = "ffmpeg -f mjpeg -i ./videos/output%d.raw -vcodec copy ./videos/%s.mp4";
static const char* deleteCmdFormat = "rm ./videos/output%d.raw";

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

// note: used C++ strings because I want the std functionalities
std::string getDateTimeStr() {
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);

    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d_%H-%M-%S");

    return oss.str();
}

void CameraControl_init() {
    CameraTrigger_init(&motionEvent);
    pthread_create(&recording_tid, NULL, recordingThread, NULL);
    pthread_create(&conversion_tid, NULL, conversionThread, NULL);
}

void CameraControl_cleanup() {
    pthread_join(recording_tid, NULL);
    pthread_join(conversion_tid, NULL);
    CameraTrigger_cleanup(&motionEvent);
}

void incrementVideo() {
    // keep the range within 10
    vidIdx = (vidIdx + 1) % 10;
    deleteIdx = (deleteIdx + 1) % 10;
}

int getPrevVideo() {
    return (vidIdx + 9) % 10;
}

static void* recordingThread(void*) {
    char recordCmd[MAX_STR_LEN];
    char deleteCmd[MAX_STR_LEN];

    while(true) {
        sprintf(recordCmd, recordCmdFormat, vidIdx);
        sprintf(deleteCmd, deleteCmdFormat, deleteIdx);
        runCommand(recordCmd);
        runCommand(deleteCmd);
        incrementVideo();
    }
}

static void* conversionThread(void*) {
    char convertCmd[MAX_STR_LEN];

    while (true) {
        event_wait(&motionEvent);
        std::string datetime_str = getDateTimeStr();
        const char* datetime_cstr = datetime_str.c_str();
        sprintf(convertCmd, convertCmdFormat, vidIdx, datetime_cstr);
        runCommand(convertCmd);
    }
}