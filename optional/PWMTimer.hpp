#pragma once
#include <stdint.h>

class PWMTimer {
public:
    PWMTimer(); // Constructor sets up the hardware
    void setDutyCycle(uint8_t duty); // Sets the output
};