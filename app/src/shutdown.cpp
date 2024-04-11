#include "shutdown.h"

static bool isShutDown = false;
static pthread_mutex_t shutdownMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t shutdownCondVar = PTHREAD_COND_INITIALIZER;

void Shutdown_init() {
    isShutDown = false;
}

void Shutdown_cleanup() {
    isShutDown = true;
}

void Shutdown_triggerShutdown() {
    pthread_mutex_lock(&shutdownMutex);
    {
        // Signal to all other clients that its time to shutdown
        pthread_cond_signal(&shutdownCondVar);
    }
    isShutDown = true;
    pthread_mutex_unlock(&shutdownMutex);
}

bool Shutdown_isShutdown() {
    return isShutDown;
}

void Shutdown_waitingShutdown() {
    pthread_mutex_lock(&shutdownMutex);
    {
        pthread_cond_wait(&shutdownCondVar, &shutdownMutex);
    }
    pthread_mutex_unlock(&shutdownMutex);
}
