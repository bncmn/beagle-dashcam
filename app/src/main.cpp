#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hal/microphone.h"
#include "hal/motion_sensor.h"

MotionEvent motionEvent;

int main() {
    // while (true) {
    //     int microphoneVoltage = Microphone_getVoltage6Reading();
    //     printf("Microphone voltage = %d\n", microphoneVoltage);
    // }

    // test code for motion sensor event
    MotionSensor_init(&motionEvent);
    while (true) {
        event_wait(&motionEvent);
        printf("Turn on the camera now! (pretend this is from the camera module)\n");
    }
    MotionSensor_cleanup(&motionEvent);

    return 0;
}