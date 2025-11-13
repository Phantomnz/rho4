#include "PWMTimer.hpp"
#include <avr/io.h> // We need this for TCCR1A, OCR1A, etc.

PWMTimer::PWMTimer() {
    // 1. Set PD5 as output
    DDRD |= _BV(PD5);

    // 2. Configure Timer1 for 8-bit Fast PWM
    // COM1A1: Clear on compare match (Non-inverting)
    // WGM10 | WGM12: Fast PWM, 8-bit
    TCCR1A = _BV(COM1A1) | _BV(WGM10);
    
    // CS11: Prescaler /8 (gives ~5.8kHz)
    TCCR1B = _BV(WGM12) | _BV(CS11);

    // 3. Initialize OCR1A bit at 0
    OCR1A = 0; 
}

void PWMTimer::setDutyCycle(uint8_t duty) {
    OCR1A = duty; // Set the duty cycle (0-255)
}