#include "config.hpp"
#include "ADCReader.hpp"
#include "PIDController.hpp"
#include "SerialPort.hpp"
#include "PWMTimer.hpp"
#include <util/delay.h>

PWMTimer g_timer;
ADCReader g_adc;
PIDController g_pid(INITIAL_KP, INITIAL_KI, INITIAL_KD); // Initial PID gains
SerialPort g_serial;

int main(void) {
    uint16_t current_setpoint = INITIAL_SETPOINT;
    uint16_t measured_value = 0;
    uint8_t pwm_output = 0;
    g_serial.sendData(current_setpoint, measured_value, pwm_output); // Initial data send
    printf("System initialized. Starting main loop...\n");
    for(;;) {
        g_serial.processIncomingData(g_pid, current_setpoint); // Check for new serial commands
        measured_value = g_adc.readADC(0); // Read the sensor value from ADC channel 0
        pwm_output =g_pid.update(current_setpoint, measured_value); // Update PID controller
        g_timer.setDutyCycle(pwm_output); // Set PWM output based on PID
        g_serial.sendData(current_setpoint, measured_value, pwm_output); // Send current data over serial back to GUI
        _delay_ms(LOOP_TIME_MS); // Wait for the next loop iteration
    }
}
