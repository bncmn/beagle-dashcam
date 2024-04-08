#ifndef _BUZZER_H_
#define _BUZZER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <stdbool.h>

#define BUZZER_ENABLE "/dev/bone/pwm/0/a/enable"
#define BUZZER_PERIOD "/dev/bone/pwm/0/a/period"
#define BUZZER_DUTY_CYCLE "/dev/bone/pwm/0/a/duty_cycle"

void Buzzer_init(void);
void Buzzer_playSound(void);

#endif