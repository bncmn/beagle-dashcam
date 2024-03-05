#include "hal/gps.h"

int serial_port;

// Taken from https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
void GPS_init() {
  serial_port = open("/dev/ttyS2", O_RDWR);
  if (serial_port < 0) {
      printf("Error %i from open: %s\n", errno, strerror(errno));
  }

  struct termios tty;
  if(tcgetattr(serial_port, &tty) != 0) {
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
  }

  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= CREAD | CLOCAL;
  tty.c_lflag |= ICANON;
  tty.c_lflag &= ~ECHO;
  tty.c_lflag &= ~ECHOE; 
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG;
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
  tty.c_oflag &= ~OPOST;
  tty.c_oflag &= ~ONLCR; 
  tty.c_cc[VTIME] = 10; // deciseconds
  tty.c_cc[VMIN] = 0;   // deciseconds
  
  // Baud Rate
  cfsetspeed(&tty, B9600);

  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
  }
}

void GPS_read() {
  char read_buf [256];
  int n = read(serial_port, &read_buf, sizeof(read_buf));

  while (true) {
    if (n > 0) {
      printf(read_buf);
    }
  }
}

void GPS_cleanup() {
  close(serial_port);
}
