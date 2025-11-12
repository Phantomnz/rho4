/*
 * Timer functions
*/
#include <avr/io.h>

// // Global interrupts must be enabled for timer to 
// // function properly
// // use sei() to enable interrupts
// void init_timer(void) {
//     TCCR0A |= _BV(WGM01);
//     // TIMSK0 |= _BV(OCIE0A);
//     TCCR1B |= ((1 << CS10) | (1 << CS12));
// }

// void delay

typedef struct {
    uint8_t scale_index;
    uint8_t duration;
    uint8_t is_beamed;
} NoteInfo;

typedef struct {
    uint8_t repeat_pos;
    uint8_t pos;
} ABCState;

typedef struct {
    uint8_t ref_no;
    uint8_t meter0;
    uint8_t meter1;
    uint8_t unit_length;
    uint8_t key;
} ABCFields;

void read_abc(const char* src, ABCState* state, NoteInfo* note) {
    state->pos = state->repeat_pos = 0;

    char node = src[state->pos] - 'A';
    switch (src[state->pos]) {
        case 'C':
        case 'c':
            
    }
}

// C D E B
// C, D, E, F, G, A, and B.
// C, G, D, A, E, B, F♯/G♭, C♯/D♭, G♯/A♭, D♯/E♭, A♯/B♭, F, C
// C, C♯, D, D♯, E, F, F♯, G, G♯, A, A♯, B, C

