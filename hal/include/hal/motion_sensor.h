// motion_sensor.h
// Module to read motion sensor to BBG
#ifndef _MOTION_SENSOR_H
#define _MOTION_SENSOR_H

#include <stdlib.h>
#include <stdio.h>

// Assume AUD pin is connected to AIN0 (pin 39)
#define A2D_FILE_VOLTAGE1  "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"

int MotionSensor_getVoltage1Reading();

#endif