#include "hal/14Seg.h"

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x20

#define REG_DIRA 0x02
#define REG_DIRB 0x03
#define REG_OUTA 0x00
#define REG_OUTB 0x01

#define LEFT_14D_VAL "/sys/class/gpio/gpio61/value"
#define RIGHT_14D_VAL "/sys/class/gpio/gpio44/value"

static unsigned char aBits[] = {0xd0, 0xc0, 0x98, 0xd8, 0xc8, 0x58, 0x58, 0x02, 0xd8, 0xd8};
static unsigned char bBits[] = {0xe1, 0x04, 0x83, 0x01, 0x22, 0x23, 0xa3, 0x05, 0xa3, 0x23};

static pthread_t displayThread;
int numToDisplay = 0;

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

static void writeToFile(char *pWriteValue, char *filePath) {
  FILE *pFile = fopen(filePath, "w");
  if (pFile == NULL) {
    printf("[ERROR] Unable to open file for write.\n");
    exit(-1);
  }

  int charWritten = fprintf(pFile, pWriteValue);
  if (charWritten <= 0) {
    printf("[ERROR] Error when writing to file.\n");
    exit(-1);
  }

  fclose(pFile);
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

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("Unable to write i2c register");
		exit(-1);
	}
}

static void writeToReg(unsigned char aBit, unsigned char bBit) {
  int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

	writeI2cReg(i2cFileDesc, REG_OUTA, aBit);
	writeI2cReg(i2cFileDesc, REG_OUTB, bBit);

  close(i2cFileDesc);
}

void Display_init(void) {
  if(!opendir("/sys/class/gpio/gpio61"))
    writeToFile("61", "/sys/class/gpio/export");
  
  if(!opendir("/sys/class/gpio/gpio44"))
    writeToFile("44", "/sys/class/gpio/export");

  sleepForMs(10);

  writeToFile("out", "/sys/class/gpio/gpio61/direction");
  writeToFile("out", "/sys/class/gpio/gpio44/direction");

  runCommand("config-pin p9_17 i2c");
  runCommand("config-pin p9_18 i2c");

  int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

	writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
	writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);

  close(i2cFileDesc);

  // pthread_create(&displayThread, NULL, Display_drive, NULL);
}

// void *Display_drive(void* unused) {
//   (void)unused;
//   while (true){
    
//     // if (numToDisplay > 99) {
//     //   numToDisplay = 99;
//     // }

//     //int firstDigit = (numToDisplay >= 10) ? (numToDisplay / 10) : 0;
//     int secondDigit = numToDisplay % 10;
    
//     // writeToFile("0", LEFT_14D_VAL);
//     // writeToFile("0", RIGHT_14D_VAL);

//     // writeToReg(aBits[firstDigit], bBits[firstDigit]);

//     // writeToFile("1", LEFT_14D_VAL);
//     // sleepForMs(5);

//     // writeToFile("0", LEFT_14D_VAL);
//     writeToFile("0", RIGHT_14D_VAL);

//     writeToReg(aBits[secondDigit], bBits[secondDigit]);

//     writeToFile("1", RIGHT_14D_VAL);
//     // sleepForMs(5);
//   }

//   // writeToFile("0", LEFT_14D_VAL);
//   writeToFile("0", RIGHT_14D_VAL);

//   return NULL;
// }

void Display_set(int num) {
  // numToDisplay = num;
  
  int secondDigit = num % 10;
  writeToFile("0", RIGHT_14D_VAL);
  writeToReg(aBits[secondDigit], bBits[secondDigit]);
  writeToFile("1", RIGHT_14D_VAL);
}

void Display_cleanup(void) {
    writeToFile("0", LEFT_14D_VAL);
    writeToFile("0", RIGHT_14D_VAL);

    // pthread_join(displayThread, NULL);
}
