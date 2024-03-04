#include "hal/motion_sensor.h"

static pthread_t motion_sensor_tid;
static void* detectMotion(void*);

int MotionSensor_getVoltage1Reading() {
    // Open file
	FILE *f = fopen(A2D_FILE_VOLTAGE1, "r");
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

void event_trigger(MotionEvent *event) {
    pthread_mutex_lock(&event->mutex);
    event->flag = 1;
    pthread_cond_signal(&event->cond);
    pthread_mutex_unlock(&event->mutex);
}

void event_wait(MotionEvent *event) {
    pthread_mutex_lock(&event->mutex);
    while (!event->flag) {
        pthread_cond_wait(&event->cond, &event->mutex);
    }
    event->flag = 0;
    pthread_mutex_unlock(&event->mutex);
}

static void* detectMotion(void *arg) {
	MotionEvent *event = (MotionEvent *)arg;
	// replace with shutdown condition ;ater
	while (true) {
		if (MotionSensor_getVoltage1Reading() > 4000) {
			printf("Motion detected!\n");
			event_trigger(event);
			sleep(3);
			printf("Timeout period ended\n\n");
		}
	}
}

void MotionSensor_init(MotionEvent *event) {
	pthread_mutex_init(&event->mutex, NULL);
    pthread_cond_init(&event->cond, NULL);
    event->flag = 0;
	pthread_create(&motion_sensor_tid, NULL, detectMotion, (void *)event);
}

void MotionSensor_cleanup(MotionEvent *event) {
    pthread_join(motion_sensor_tid, NULL);
    pthread_mutex_destroy(&event->mutex);
    pthread_cond_destroy(&event->cond);
}