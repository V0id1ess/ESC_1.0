#include <initializations.h>

void initESC() {
    // Initialize clocks
    enableSystemClock();

    // Enable Peripherals
    configureLEDs();

    // Configure PWM pins
    enablePWMs();
    configurePWMs();

    // Configure Phase Control ADC pins
    configureADCs();

    // Configure Signal Input ADC pin

}

int main() {
    // Setup
    initESC();
    
    // Loop
    while (true) {

    }
}