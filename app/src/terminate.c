#include "terminate.h"

bool terminateProgram = false;

void setTerminate() {
   terminateProgram = true;
}

bool isTerminate() {
  return terminateProgram;
}