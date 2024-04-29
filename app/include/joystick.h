#ifndef JOYSTIC_H
#define JOYSTICK_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define GPIO_BASE_PATH "/sys/class/gpio/"
#define BUFFER_MAX_LENGTH 1024

FILE *openJoystickFile(int pin, const char *mode);

void writeJoystickFile(int pin);

bool readJoystickFile(int pin);

bool anyDirectionPressed(void);

void initPin(int pin);

void initAllPins(void);

void joystick_init();

void joystick_cleanup();

#endif