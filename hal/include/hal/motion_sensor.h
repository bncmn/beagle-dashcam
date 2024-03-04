// motion_sensor.h
// Module to read motion sensor to BBG
#ifndef _MOTION_SENSOR_H
#define _MOTION_SENSOR_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Assume AUD pin is connected to AIN0 (pin 39)
#define A2D_FILE_VOLTAGE1  "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int flag;
} MotionEvent;

void MotionSensor_init(MotionEvent *event);
void MotionSensor_cleanup(MotionEvent *event);
void event_wait(MotionEvent *event);

#endif