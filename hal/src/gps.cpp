#include "hal/gps.h"

int serial_port;

// function given by Brian Fraser
static void runCommand(const char* command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        printf("--> %s", buffer); // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

// Taken from https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
void GPS_init() {
  runCommand("config-pin p9.21 uart");
  runCommand("config-pin p9.22 uart");

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
  char read_buf[255];
  int counter = 0;

  while (true) {
    int n = read(serial_port, &read_buf, sizeof(read_buf));
    if (n > 0) {
      if (strncmp(read_buf, "$GNGGA", 6) == 0) {
        char *token = strtok(read_buf, ",");
        int lineIndex = 0;
        char temp[128] = "";
        while (token != NULL) {
          if (lineIndex == LAT_MAGNITUDE || lineIndex == LON_MAGNITUDE) {
            double coord = atof(token);
            int degrees = static_cast<int>(coord / 100);
            double minutes = coord - degrees * 100;
            double decimal_degrees = degrees + minutes / 60.0;
            sprintf(temp + strlen(temp), "%.6f ", decimal_degrees);
          } else if (lineIndex == LAT_HEMISPHERE || lineIndex == LON_HEMISPHERE) {
            strcat(temp, token);
            strcat(temp, " ");
          }
          token = strtok(NULL, ",");
          lineIndex++;
        }
        
        counter++;
        printf("FETCH #%d: %s\n", counter, temp);
      }
    }
  }
}

void GPS_cleanup() {
  close(serial_port);
}
