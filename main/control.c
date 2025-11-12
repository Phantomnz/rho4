#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

// --- Controller Definitions ---
// Let's set our target voltage to 1.65V
// (1.65V / 3.3V) * 1023 = 511.5. We'll use 512.
#define SETPOINT 512

// This is the "Proportional Gain" (Kp).
// It's the "strength" of our correction.
// We will have to "tune" this value. Let's start with 0.1.
#define KP 0.1 

// How long to wait in each loop (in ms)
#define LOOP_TIME_MS 10

// --- Function Prototypes ---
void init_pwm_dac(void);
void init_adc(void);
uint16_t read_adc(void);

// --- Main Program ---
int main(void) {
    // 1. Initialize all hardware
    init_debug_uart0();
    init_pwm_dac();
    init_adc();
    
    printf("--- Closed-Loop P-Controller ---\n");
    printf("Setpoint: %u (approx 1.65V)\n", SETPOINT);

    for (;;) {
        // 2. Read the actual voltage (Process Variable)
        uint16_t measured_value = read_adc();

        // 3. Calculate the error (Setpoint - Measured)
        // Use a signed int because error can be negative!
        int16_t error = SETPOINT - measured_value;

        // 4. Calculate the P-Term
        // This is our control output
        double p_term = KP * (double)error;

        // 5. Get the current PWM value (0-255)
        // We read OCR1A because it holds the last value we set
        double current_pwm = OCR1A;
        
        // 6. Calculate new PWM
        // The new PWM is the last one + the correction
        double new_pwm_double = current_pwm + p_term;

        // 7. Clamp (Saturate) the output
        // The PWM value MUST be between 0 and 255
        if (new_pwm_double > 255.0) {
            new_pwm_double = 255.0;
        } else if (new_pwm_double < 0.0) {
            new_pwm_double = 0.0;
        }

        // 8. Apply the output
        uint8_t new_pwm_int = (uint8_t)new_pwm_double;
        OCR1A = new_pwm_int;

        // 9. Print debug info
        printf("Measured: %u, Error: %d, PWM: %u\n", 
               measured_value, error, new_pwm_int);

        // 10. Wait for the next loop
        _delay_ms(LOOP_TIME_MS);
    }
}

// --- Function Definitions ---

void init_pwm_dac(void) {
    // Copied from dac.c
    DDRD |= _BV(PD5);
    TCCR1A = _BV(WGM10) | _BV(COM1A1);
    TCCR1B = _BV(CS11) | _BV(WGM12);
}

void init_adc(void) {
    // Copied from adc.c
    DDRA &= ~_BV(PA0); // Corrected pin
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
    ADMUX = 0; // Set to PA0
}

uint16_t read_adc(void) {
    // Copied from adc.c
    ADCSRA |= _BV(ADSC);
    while ((ADCSRA & _BV(ADSC)));
    return ADC;
}