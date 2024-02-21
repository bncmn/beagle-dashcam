// microphone.h
// Module to read microphone amplifier to BBG
#ifndef _MICROPHONE_H
#define _MICROPHONE_H

#include <stdlib.h>
#include <stdio.h>

// Assume AUD pin is connected to AIN6 (pin 35)
#define A2D_FILE_VOLTAGE6  "/sys/bus/iio/devices/iio:device0/in_voltage6_raw"

int Microphone_getVoltage6Reading();

#endif