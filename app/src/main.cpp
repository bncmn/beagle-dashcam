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
#include "udpNetwork.h"
#include "hal/sdCard.h"
#include "joystick.h"
#include "shutdown.h"

int main() {
  printf("Launching BeagleDashCam...\n");
  unmountSDCard();
  mountSDCard();
  GPS_init();
  Buzzer_init();
  Display_init();
  Acceleromerter_init();
  Udp_init();
  CameraControl_init();
  initAllPins();
  Shutdown_init();

  Shutdown_waitingShutdown();

  Shutdown_cleanup();
  CameraControl_cleanup();
  unmountSDCard();
  // GPS_cleanup();
  
  return 0;
}