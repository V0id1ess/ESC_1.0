#include <initializations.h>

void initESC() {
    // Initialize & Configure clocks
    enableSystemClock();
    configureClocks();

    // Enable FPU
    enableFPU();

    // Enable Debug LEDs
    configureLEDs();

    // Configure Phase Control ADC pins
    configureADC();

    // Configure PWM pins
    enablePWM();
    configurePWM();

    // Configure Interrupts
    configureInterrupts();
}

int main() {
    // Setup
    initESC();
    
    // Loop
    while (true) {

    }
}