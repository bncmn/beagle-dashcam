#include "hal/cameraTrigger.h"
#include "terminate.h"

// Assume AUD pin is connected to AIN0 (pin 39)
const char* A2D_FILE_VOLTAGE1 = "/sys/bus/iio/devices/iio:device0/in_voltage1_raw";
// assume button is wired to p9.15
const char* BUTTON = "/sys/class/gpio/gpio48/value";

static pthread_t motion_sensor_tid;
static pthread_t button_tid;
static void* detectMotion(void*);
static void* detectButtonPress(void*);

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

int getReading(const char* path) {
    // Open file
	FILE *f = fopen(path, "r");
	if (!f) {
		printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
		printf("       Check /boot/uEnv.txt for correct options.\n");
		exit(-1);
	}

	// Get reading
	int a2dReading = 0;
	int itemsRead = fscanf(f, "%d", &a2dReading);
	if (itemsRead <= 0) {
		printf("ERROR: Unable to read values from voltage input file.\n");
		exit(-1);
	}

	// Close file
	fclose(f);

	return a2dReading;
}

void event_trigger(CameraEvent *event) {
    pthread_mutex_lock(&event->mutex);
    event->flag = 1;
    pthread_cond_signal(&event->cond);
    pthread_mutex_unlock(&event->mutex);
}

void event_wait(CameraEvent *event) {
    pthread_mutex_lock(&event->mutex);
    while (!event->flag) {
        pthread_cond_wait(&event->cond, &event->mutex);
    }
    event->flag = 0;
    pthread_mutex_unlock(&event->mutex);
}

static void* detectMotion(void *arg) {
	CameraEvent *event = (CameraEvent *)arg;
	// replace with shutdown condition later
	while (true) {
		if (isTerminate()) {
            break;
        }
		if (getReading(A2D_FILE_VOLTAGE1) > 4000) {
			event_trigger(event);
			sleep(3);
			printf("Timeout period ended\n");
		}
	}
}

static void* detectButtonPress(void *arg) {
	CameraEvent *event = (CameraEvent *)arg;
	// replace with shutdown condition later
	while (true) {
		if (isTerminate()) {
            break;
        }
		if (getReading(BUTTON)) {
			event_trigger(event);
			sleep(1);
			printf("Timeout period ended\n");
		}
	}
}

void CameraTrigger_init(CameraEvent *event) {
	runCommand("config-pin p9.15 gpio");
	pthread_mutex_init(&event->mutex, NULL);
    pthread_cond_init(&event->cond, NULL);
    event->flag = 0;
	pthread_create(&motion_sensor_tid, NULL, detectMotion, (void *)event);
	pthread_create(&button_tid, NULL, detectButtonPress, (void *)event);
}

void CameraTrigger_cleanup(CameraEvent *event) {
    pthread_join(motion_sensor_tid, NULL);
	pthread_join(button_tid, NULL);
    pthread_mutex_destroy(&event->mutex);
    pthread_cond_destroy(&event->cond);
}