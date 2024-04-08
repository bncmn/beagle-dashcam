#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hal/microphone.h"
#include "hal/cameraTrigger.h"
#include "hal/gps.h"
#include "hal/buzzer.h"
#include "hal/accelerometer.h"
#include "hal/14Seg.h"
#include "cameraControl.h"

int main() {
  printf("Launching BeagleDashCam...\n");
  GPS_init();
  Buzzer_init();
  Display_init();
  Acceleromerter_init();
  CameraControl_init();


  CameraControl_cleanup();
  // GPS_cleanup();
  
  return 0;
}