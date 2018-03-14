#include <avr/interrupt.h>

#include "timer.h"
#include "pwm.h"


volatile bool need_new_sample;
volatile uint8_t pwm_val;
volatile uint32_t timestamp;

void timer_init(void) {
	TCNT2  = 0x00;			  // set timer2 to 0
	TCCR2A = (1 << WGM21);    // CTC Mode
		
	TCCR2B = (1 << CS21);     // prescaler 8 (16MHz / 8 = 2MHz)
	OCR2A = 128; 			  // set output compare register to 128, timer reset interval is (2Mhz / 128 = 15625Hz)
	
	TIMSK2 |= (1 << OCIE2A);  // enable TIMER2 Output Compare Match A Interrupt
}
/**
SIGNAL(TIMER2_COMPA_vect) {
	pwm_set_pulsewidth(pwm_val);
	timestamp += 1;
	need_new_sample = true;
}
**/
uint32_t get_timestamp(void) {
	uint32_t retval;
	cli();
	retval = timestamp;
	sei();
	return retval;
}
