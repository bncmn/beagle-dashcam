#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "hal/microphone.h"

int main() {
    while (true) {
        int microphoneVoltage = Microphone_getVoltage6Reading();
        printf("Microphone voltage = %d\n", microphoneVoltage);
    }

    return 0;
}