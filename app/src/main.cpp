#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hal/microphone.h"
#include "hal/motion_sensor.h"


int main() {
    // while (true) {
    //     int microphoneVoltage = Microphone_getVoltage6Reading();
    //     printf("Microphone voltage = %d\n", microphoneVoltage);
    // }

    while (true) {
        int msVoltage = MotionSensor_getVoltage1Reading();
        printf("Motion sensor voltage = %d\n", msVoltage);
        usleep(10000);
    }

    return 0;
}