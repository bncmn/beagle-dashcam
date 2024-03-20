#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hal/microphone.h"
#include "hal/cameraTrigger.h"
#include "hal/gps.h"

CameraEvent motionEvent;

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

int main() {
    // while (true) {
    //     int microphoneVoltage = Microphone_getVoltage6Reading();
    //     printf("Microphone voltage = %d\n", microphoneVoltage);
    // }

    // test code for triggering camera event; try hooking it up with the camera later and turning up the timeout
    CameraTrigger_init(&motionEvent);
    while (true) {
        event_wait(&motionEvent);
        //printf("Turn on the camera now! (pretend this is from the camera module)\n");

        // calling camera on event. CMake (hopefully) should be copying capture to the same directory as bbg_dashcam in order for these to work.
        runCommand("./capture -F -c 300 -o > output.raw");
        runCommand("ffmpeg -f mjpeg -i output.raw -vcodec copy output.mp4");
    }
    CameraTrigger_cleanup(&motionEvent);

    // GPS_init();
    // GPS_read();
    // GPS_cleanup();

    return 0;
}