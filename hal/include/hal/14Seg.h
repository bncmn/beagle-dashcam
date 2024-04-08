#ifndef _14_SEG_H_
#define _14_SEG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include <dirent.h>

// Display_init initializes the 14-segment display.
// Pins 61 and 44 are confiured to output through GPIO.
// The I2C bus is enabled on P9.17 and P9.18.
// A Display_drive thread is created here, which will drive the display as long as the program runs.
void Display_init(void);

// Display_drive writes the desired number on the display.
// This is the "main" display thread logic.
// void *Display_drive(void *unused);

// Display_set sets the number to be written on the display.
void Display_set(int num);

// Display_cleanup cleans up and turns off the display.
void Display_cleanup(void);

#endif