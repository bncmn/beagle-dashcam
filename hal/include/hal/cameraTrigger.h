// motion_sensor.h
// Module to read motion sensor to BBG
#ifndef _MOTION_SENSOR_H
#define _MOTION_SENSOR_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "hal/accelerometer.h"

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int flag;
} CameraEvent;

void CameraTrigger_init(CameraEvent *event);
void CameraTrigger_cleanup(CameraEvent *event);
void event_wait(CameraEvent *event);

void CameraTrigger_turnSensorOn();
void CameraTrigger_turnSensorOff();

#endif