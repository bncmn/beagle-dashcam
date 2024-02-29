#ifndef HELPER_H
#define HELPER_H

#include <stdbool.h>

// loopProgram is accessible to all modules by importing this header file.
// UDP, sampler, and display threads watch this value to detect program termination.
extern bool loopProgram;

// runCommand takes in a string command and runs that string in a UNIX command prompt.
void runCommand(char* command);

// writeToFile writes a string pWriteValue into the file specified by filePath.
void writeToFile(char *pWriteValue, char *filePath);

// sleepForMs pauses program execution for time delayInMs.
void sleepForMs(long long delayInMs);

#endif