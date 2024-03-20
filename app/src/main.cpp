#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hal/microphone.h"
#include "hal/cameraTrigger.h"
#include "hal/gps.h"
#include "cameraControl.h"

CameraEvent motionEvent;

int main() {
    // while (true) {
    //     int microphoneVoltage = Microphone_getVoltage6Reading();
    //     printf("Microphone voltage = %d\n", microphoneVoltage);
    // }

    // test code for triggering camera event; try hooking it up with the camera later and turning up the timeout
    CameraTrigger_init(&motionEvent);
    CameraControl_init();

    CameraControl_cleanup();
    CameraTrigger_cleanup(&motionEvent);

    // GPS_init();
    // GPS_read();
    // GPS_cleanup();

    return 0;
}