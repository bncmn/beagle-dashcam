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

#include "tools.h"
#include "accelometer.h"

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x18

#define REG_X0_DATA 0x28
#define REG_X1_DATA 0x29
#define REG_Y0_DATA 0x2A
#define REG_Y1_DATA 0x2B
#define REG_Z0_DATA 0x2C
#define REG_Z1_DATA 0x2D

// 2 times the acceleration due to gravity scale
#define REG_CONFIG 0x20
#define CONFIG_SETTINGS 0x27

static int initI2cBus(char* bus, int address)
{
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(-1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		perror("Unable to set I2C device to slave address.");
		exit(-1);
	}
	return i2cFileDesc;
}

static unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr) { 
	// To read a register, must first write the address 
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr)); 
	if (res != sizeof(regAddr)) { 
		perror("I2C: Unable to write to i2c register."); 
		exit(1); 
	}

	// Now read the value and return 
	it char value = 0; res = read(i2cFileDesc, &value, sizeof(value)); 
	if (res != sizeof(value)) { 
		perror("I2C: Unable to read from i2c register"); 
		exit(1); 
	} 
	return value; 
}

// ******************************************************
// *          NON-STATIC FUNCTIONS BEGIN HERE!          *
// ******************************************************

void accel_init() {
	runCommand("config-pin p9_17 i2c");
  runCommand("config-pin p9_18 i2c");
	runCommand("i2cset -y 1 0x18 0x20 0x27");
}

void readReg(unsigned char aBit, unsigned char bBit) {
  int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

	int x0 = readI2cReg(i2cFileDesc, REG_X0_DATA);
	int x1 = readI2cReg(i2cFileDesc, REG_X1_DATA);

	int y0 = readI2cReg(i2cFileDesc, REG_Y0_DATA);
	int y1 = readI2cReg(i2cFileDesc, REG_Y1_DATA);

	int z0 = readI2cReg(i2cFileDesc, REG_Z0_DATA);
	int z1 = readI2cReg(i2cFileDesc, REG_Z1_DATA);

  close(i2cFileDesc);
}