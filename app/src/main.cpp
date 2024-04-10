#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hal/microphone.h"
#include "hal/cameraTrigger.h"
#include "hal/gps.h"
#include "cameraControl.h"
#include "hal/sdCard.h"
#include "hal/joystick.h"
#include "terminate.h"

int main() {
    // while (true) {
    //     int microphoneVoltage = Microphone_getVoltage6Reading();
    //     printf("Microphone voltage = %d\n", microphoneVoltage);
    // }

    mountSDCard();
    initJoystickDown();

    CameraControl_init();

    CameraControl_cleanup();

    unmountSDCard();

    // GPS_init();
    // GPS_read();
    // GPS_cleanup();

    return 0;
}