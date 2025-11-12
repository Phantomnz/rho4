#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

const uint8_t SEGMENTS[10] = {
    0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6
};

int main(void) {
    // uart setup
    init_debug_uart0();
    DDRB |= _BV(PINB7);
    DDRA = 0xFF;
    // PORTA = 0xBE;

    PORTC |= _BV(7);

    uint8_t digit = 0;

    while (1) {
        // PORTA = SEGMENTS[digit];
        digit = (digit + 1) % 10;

        // only bit using UART
        printf("%u", digit);

        PORTB |= _BV(PINB7);
        _delay_ms(700);
        PORTB &= ~_BV(PINB7);
        _delay_ms(300);
    }
}