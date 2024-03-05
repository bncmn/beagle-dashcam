#ifndef _GPS_H
#define _GPS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

void GPS_init(void);
void GPS_read(void);
void GPS_cleanup(void);

#endif