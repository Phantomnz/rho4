#include "AVRSerial.hpp"
#include <avr/io.h> // For USART registers and bit definitions
#include <stdio.h>  // For sprintf/sscanf

AVRSerial::AVRSerial() : m_bufferIndex(0) {
	// Configure UART0 baud rate (assuming F_CPU is defined in makefile)
    #define BAUD 9600
    #include <util/setbaud.h> // Helper to calculate UBRR values
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    
    // Enable RX and TX
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    // 8-bit data, 1 stop bit
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

void AVRSerial::sendData(uint16_t setpoint, uint16_t measured, uint8_t output) {
    char buffer[64];
    // Format the string into a local buffer
    // Note: We use a simpler format "D,..." to make parsing easier on the PC side
    snprintf(buffer, sizeof(buffer), "D,%u,%u,%u\r\n", setpoint, measured, output);
    
    // Send the buffer one char at a time
    for (int i = 0; buffer[i] != '\0'; i++) {
        while (!(UCSR0A & _BV(UDRE0))); // Wait for TX buffer empty
        UDR0 = buffer[i];               // Send char
    } 
}

void AVRSerial::processIncomingData(PIDController& pid, uint16_t& setpoint) {
    // 1. Check if data is available (Non-blocking!)
    if (UCSR0A & _BV(RXC0)) {
        char c = UDR0; // Read the character

        // 2. Check for end of line
        if (c == '\n' || c == '\r') {
            m_buffer[m_bufferIndex] = '\0'; // Null-terminate
            parseCommand(pid, setpoint);    // Process the full command
            m_bufferIndex = 0;              // Reset buffer
        } else {
            // 3. Add to buffer (if safe)
            if (m_bufferIndex < sizeof(m_buffer) - 1) {
                m_buffer[m_bufferIndex++] = c;
            }
        }
    }
}

void AVRSerial::parseCommand(PIDController& pid, uint16_t& setpoint) {
    char type = m_buffer[0];
    double val_d;
    uint16_t val_u;

    // Protocol: "p0.5", "i0.1", "d0.05", "s512"
    if (type == 's') {
        if (sscanf(&m_buffer[1], "%u", &val_u) == 1) {
            setpoint = val_u;
        }
    } 
    // Note: We need to use a temporary variable for gains because we can't
    // get the current gains out of the PID controller easily without adding getters.
    // A cleaner way is to just update the specific gain if your PID class supports it,
    // Inside parseCommand...
    else if (type == 'p') {
        if (sscanf(&m_buffer[1], "%lf", &val_d) == 1) {
            // Use getters to keep the other values the same!
            pid.setGains(val_d, pid.getKi(), pid.getKd());
        }
    }
    else if (type == 'i') {
        if (sscanf(&m_buffer[1], "%lf", &val_d) == 1) {
            pid.setGains(pid.getKp(), val_d, pid.getKd());
        }
    }
    else if (type == 'd') {
        if (sscanf(&m_buffer[1], "%lf", &val_d) == 1) {
            pid.setGains(pid.getKp(), pid.getKi(), val_d);
        }
    }

}

