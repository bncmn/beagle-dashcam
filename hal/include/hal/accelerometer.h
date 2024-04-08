#ifndef _ACCELEROMETER_H
#define _ACCELEROMETER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdbool.h>
#include <pthread.h>

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x18

#define REG_XL 0x28
#define REG_XH 0x29

#define REG_YL 0x2a
#define REG_YH 0x2b

#define REG_ZL 0x2c
#define REG_ZH 0x2d

#define XY_ABS_THRESHOLD 17000
#define Z_ABS_THRESHOLD 30000

void Acceleromerter_init();
bool Accelerometer_checkThreshold();

#endif