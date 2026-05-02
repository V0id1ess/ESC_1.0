#include <initializations.h>
#include <control.h>

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

    // Configure DShot Output
    
}

int main() {
    // Setup
    initESC();
    
    // Loop
    while (true) {

    }
}