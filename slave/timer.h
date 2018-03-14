#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

#ifndef F_CPU
#warning "adjust OCR2A and set F_CPU"
#define F_CPU 16000000UL
#endif

extern volatile bool need_new_sample;
extern volatile uint8_t pwm_val;
void timer_init(void);
uint32_t get_timestamp(void);

#endif
