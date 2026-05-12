#include <esc.h>

void initESC() {
    // Initialize & Configure clocks
    enableSystemClock();
    configureClocks();

    // Enable Debug LEDs
    configureLEDs();

    // Enable FPU
    enableFPU();

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
        // *((volatile uint32_t*) (GPIOA + GPIO_BRR)) |= (1U << 0); // PA0 LOW
        // for (volatile unsigned long i = 0; i < SYSCLK/13 / 2; i++) { // ~0.5s delay at 64MHz
        //     __asm__ volatile ("nop");
        // }
        // *((volatile uint32_t*) (GPIOA + GPIO_BSRR)) |= (1U << 0); // PA0 HIGH
        // for (volatile unsigned long i = 0; i < SYSCLK/13 / 2; i++) { // ~0.5s delay at 64MHz
        //     __asm__ volatile ("nop");
        // }
    }
}