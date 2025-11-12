/*  Author: Klaus-Peter Zauner
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *          View this license at http://creativecommons.org/about/licenses/
 *   Notes: F_CPU must be defined to match the clock frequency
 */
#include <avr/io.h>
#include <util/delay.h>
#include "et_scale.h"

#define FREQ 262

/*
 PIN | PORT | Description
 PD5 |  D   | Tone oscillator output
*/



unsigned long toneClock;   /* set at timer initialisation and used
                              to calculate TOP when setting frequency */ 

void init_tone(void);
void tone(uint16_t frequency);

int main(void) {

    init_tone();
    
    for (;;) {
        uint8_t i = 0;
        for (; i < ET_SCALE_TOP; i++) {
            tone(et_scale[i]);
            _delay_ms(150);
        }
        for (i -= 1; i > 0; i--) {
            tone(et_scale[i]);
            _delay_ms(150);
        }
    }
    

}



/* init_tone ---
   Setup timer as a tone frequency oscillator.
*/
void init_tone(void)
{
    DDRD = _BV(PD5);
    TCCR1A |= _BV(COM1A0) | _BV(WGM10);
	TCCR1B |= _BV(CS11) | _BV(WGM13);
    toneClock = 12000000 / (8 * 2 * 2);
}

/* tone ---
   Set oscillator output to desired frequency
*/
void tone(uint16_t frequency)
{
    uint16_t top = toneClock / frequency;
	// unsigned char sreg; /* Save global interrupt flag */ sreg = SREG; /* Disable interrupts */
	// _CLI();
	OCR1A = top;
    // SREG = sreg;
}

