#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void pwm_init(void);

void pwm_set_pulsewidth(uint8_t width);

#endif
