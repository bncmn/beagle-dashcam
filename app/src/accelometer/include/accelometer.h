#ifndef _ACCELOMETER_H_
#define _ACCELOMETER_H_

// Display_init initializes the 14-segment display.
// Pins 61 and 44 are confiured to output through GPIO.
// The I2C bus is enabled on P9.17 and P9.18.
// A Display_drive thread is created here, which will drive the display as long as the program runs.

// void Display_init(void);


// Display_drive writes the desired number on the display.
// This is the "main" display thread logic.

// void *Display_drive(void *unused);


// Display_set sets the number to be written on the display.

// void Display_set(int num);


// Display_cleanup cleans up and turns off the display.

// void Display_cleanup(void);

#endif