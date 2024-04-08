#include "hal/accelerometer.h"

int i2cFileDesc;

static void runCommand(char* command) {
  // Execute the shell command (output into pipe)
  FILE *pipe = popen(command, "r");
  
  // Ignore output of the command; but consume it
  // so we don't get an error when closing the pipe.
  char buffer[1024];
  while (!feof(pipe) && !ferror(pipe)) {
    if (fgets(buffer, sizeof(buffer), pipe) == NULL)
    break;
    // printf("--> %s", buffer); // Uncomment for debugging
  }
  
  // Get the exit code from the pipe; non-zero is an error:
  int exitCode = WEXITSTATUS(pclose(pipe));
  if (exitCode != 0) {
    perror("Unable to execute command:");
    printf(" command: %s\n", command);
    printf(" exit code: %d\n", exitCode);
  }
}

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

static unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		exit(-1);
	}

	// Now read the value and return it
	char value = 0;

  res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		exit(-1);
	}
	return value;
}

static int16_t* convertTo16bit(int i2cFileDesc) {
  static int16_t values[3];

  unsigned char X_MSB = readI2cReg(i2cFileDesc, REG_XH);
  unsigned char X_LSB = readI2cReg(i2cFileDesc, REG_XL);
  unsigned char Y_MSB = readI2cReg(i2cFileDesc, REG_YH);
  unsigned char Y_LSB = readI2cReg(i2cFileDesc, REG_YL);
  unsigned char Z_MSB = readI2cReg(i2cFileDesc, REG_ZH);
  unsigned char Z_LSB = readI2cReg(i2cFileDesc, REG_ZL);

  values[0] = ((X_MSB << 8) | X_LSB);
  values[1] = ((Y_MSB << 8) | Y_LSB);
  values[2] = ((Z_MSB << 8) | Z_LSB);

  return values;
}

static void sleepForMs(long long delayInMs) {
  const long long NS_PER_MS = 1000 * 1000;
  const long long NS_PER_SECOND = 1000000000;
  long long delayNs = delayInMs * NS_PER_MS;
  int seconds = delayNs / NS_PER_SECOND;
  int nanoseconds = delayNs % NS_PER_SECOND;
  struct timespec reqDelay = {seconds, nanoseconds};
  nanosleep(&reqDelay, (struct timespec *) NULL);
}

void Acceleromerter_init() {
  runCommand("config-pin p9.17 i2c");
  runCommand("config-pin p9.18 i2c");
  runCommand("i2cset -y 1 0x18 0x20 0x27");
  i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
}

bool Accelerometer_checkThreshold() {  
  int16_t* vals = convertTo16bit(i2cFileDesc);

  if (vals[0] >= XY_ABS_THRESHOLD || vals[0] <= -XY_ABS_THRESHOLD) {
    return true;
  }
  if (vals[1] >= XY_ABS_THRESHOLD || vals[1] <= -XY_ABS_THRESHOLD) {
    return true;
  } 
  if (vals[2] >= Z_ABS_THRESHOLD || vals[2] <= -Z_ABS_THRESHOLD) {
    return true;
  }

  return false;
}


