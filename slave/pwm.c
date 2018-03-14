#include <avr/io.h>


void pwm_init(void) {
	// non-inverted fast PWM, no prescaling, 8bit PWM resolution
	TCCR1A = (1 << WGM10) | (1 << COM1A1);
	TCCR1B = (1 << WGM12) | (1 << CS10);

	TCNT1 = 0;	  // start counter at 0
	OCR1A = 0x80; // set 50% PW

	DDRB |= (1 << PB1); // set pin B1 (OC1A) as output
}

void pwm_set_pulsewidth(uint8_t width) {
	OCR1A = width;
}
