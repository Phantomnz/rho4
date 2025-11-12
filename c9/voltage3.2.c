/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License. 
 *           View this license at http://creativecommons.org/about/licenses/
 *   Notes: F_CPU must be defined to match the clock frequency
 */
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

// Voltage present and away for proximity sensor
const double V_P = 0.644;
const double V_A = 0.980;

void init_adc(void)
{
	//       Enable ADC |   /64 clock prescaler
	ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
	DDRB |= _BV(PB7); /*LED*/
}

uint16_t read_adc(void)
{
    ADCSRA |= _BV(ADSC);
    // Busy loop while ADC Start conversion bit is set
    while ((ADCSRA & _BV(ADSC)));

    // ADSC is 0 now so ADC may be read completely.
	return ADC;
}

// n must be no greater than 7
void channel_adc(uint8_t n) {
    // Select PA3 as ADC single ended input channel
	ADMUX = n;
}

const uint16_t PLOT_WIDTH = 60;

void plotval(double d, uint16_t width) {
    // printf("%*s\n", (int)(d * width), "*");

    uint16_t spaces = (uint16_t)(d * width);
    for (uint16_t i = 0; i < spaces; i++) {
        putchar(' ');
    }
    printf("*\n");
}

int main(void)
{
	uint16_t result;
    double voltage;
	double y;
	
	init_debug_uart0();
	init_adc();
	channel_adc(1);
	
	for (;;) 
	{
        result = read_adc();
        voltage = (result / 1024.0) * 3.3;

		// if (voltage > V_A) PORTB &= ~_BV(PB7);
		// else if (voltage > V_P) PORTB |= _BV(PB7);
		
		// printf("%4d : %6.5fV\n", result, voltage);
		y = result / 1024.0;
		plotval(y, PLOT_WIDTH);
		_delay_ms(50);
	}
}