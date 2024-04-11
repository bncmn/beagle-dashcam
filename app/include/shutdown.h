// shutdown.h
// Module to handle shutdown gracefully when the UDP server exit.
// Close all open sockets, files, pipes, threads, and free dynamic memory
// Copied from Lynn Nguyen & Kevin Chen's prior assignments
#ifndef _SHUTDOWN_H_
#define _SHUTDOWN_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

// Begin/end all the background threads from other modules
void Shutdown_init();
void Shutdown_cleanup();

void Shutdown_triggerShutdown();

// Return shutdown status so other modules exit their background thread loop
bool Shutdown_isShutdown();

void Shutdown_waitingShutdown();

#endif