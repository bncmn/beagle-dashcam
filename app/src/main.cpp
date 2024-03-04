#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hal/microphone.h"
#include "hal/cameraTrigger.h"

CameraEvent motionEvent;

int main() {
    // while (true) {
    //     int microphoneVoltage = Microphone_getVoltage6Reading();
    //     printf("Microphone voltage = %d\n", microphoneVoltage);
    // }

    // test code for triggering camera event; try hooking it up with the camera later and turning up the timeout
    // CameraTrigger_init(&motionEvent);
    // while (true) {
    //     event_wait(&motionEvent);
    //     printf("Turn on the camera now! (pretend this is from the camera module)\n");
    // }
    // CameraTrigger_cleanup(&motionEvent);

    return 0;
}