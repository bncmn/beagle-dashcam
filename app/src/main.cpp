#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hal/microphone.h"
#include "hal/cameraTrigger.h"
#include "hal/gps.h"
#include "hal/buzzer.h"
#include "cameraControl.h"

int main() {
    GPS_init();
    Buzzer_init();
    CameraControl_init();

    CameraControl_cleanup();
    // GPS_cleanup();
    
    return 0;
}