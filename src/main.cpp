#include <initializations.h>

void initESC() {
    // Initialize clocks
    enableSystemClock();

    // Enable Peripherals
    enableLEDs();

    // Configure PWM pins
    enablePWMs();
    configurePWMs();

    // Configure Phase Control ADC pins

    // Configure Signal Input ADC pin

}

int main() {
    // Setup
    initESC();
    
    // Loop
    while (true) {

    }
}