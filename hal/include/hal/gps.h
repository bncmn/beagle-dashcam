#ifndef _GPS_H
#define _GPS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string>

#define LAT_MAGNITUDE 2
#define LAT_HEMISPHERE 3
#define LON_MAGNITUDE 4
#define LON_HEMISPHERE 5

void GPS_init(void);
std::string GPS_read(void);
void GPS_cleanup(void);
bool GPS_connected();

#endif