#include <stm32f303cbt6.h>
#include <common.h>

#define CLOCKSPEED = 72_000_000 // 72 MHz

void enableSystemClock() {
    *((volatile uint32_t*) (RCC + RCC_CR)) |= (1 << 0); // HSI ON
    while (! (*((volatile uint32_t*) (RCC + RCC_CR)) & (1 << 1))); // Wait for HSI to be ready

    *((volatile uint32_t*) (RCC + RCC_CFGR)) |= (0b01 << 15); // PLL source = HSI 
    *((volatile uint32_t*) (RCC + RCC_CFGR)) |= (0b0111 << 18); // PLL input clock x 9 (8MHz * 9 = 72MHz)

    *((volatile uint32_t*) (RCC + RCC_CFGR)) |= (0b10 << 0); // SYSCLK Source = PLL
    while (! (*((volatile uint32_t*) (RCC + RCC_CFGR)) & (0b10 << 2))); // Wait for SYSCLK source to be switched
}

void enableLEDs() {
    *((volatile uint32_t*) (RCC + RCC_AHBENR)) |= (1 << 17); // Enable GPIOA Clock
    *((volatile uint32_t*) (RCC + RCC_AHBENR)) |= (1 << 18); // Enable GPIOB Clock

    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b01 << 0); // PA0 as Output (LED)
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b01 << 2); // PA1 as Output (LED)

    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << 0); // PA0 High Speed
    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << 2); // PA1 High Speed
    // Set PA0 to High
    *((volatile uint32_t*) (GPIOA + GPIO_BSRR)) |= (1 << 0); // PA0 High
    // Set PA1 to Low
    *((volatile uint32_t*) (GPIOA + GPIO_BRR)) |= (1 << 1); // PA1 Low
}

void enablePWMs() {
    // Enable TIM1 Clock
    *((volatile uint32_t*) (RCC + RCC_APB2ENR)) |= (1 << 11); // TIM1EN

    // Configure TIM1 for PWM
    

    // Configure Alternate Functions
    // PA8 - TIM1_CH1; PA9 - TIM1_CH2; PA10 - TIM1_CH3
    // PA11 - TIM1_CH1N; PA12 - TIM1_CH2N; PB1 - TIM1_CH3N
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b10 << (8 * 2)); // PA8 AF
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b10 << (9 * 2)); // PA9 AF
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b10 << (10 * 2)); // PA10 AF
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b10 << (11 * 2)); // PA11 AF
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b10 << (12 * 2)); // PA12 AF
    *((volatile uint32_t*) (GPIOB + GPIO_MODER)) |= (0b10 << (1 * 2)); // PB1 AF

    // Set AFR
    *((volatile uint32_t*) (GPIOA + GPIO_AFRH)) |= (6 << ((8 - 8) * 4)); // PA8 AF6
    *((volatile uint32_t*) (GPIOA + GPIO_AFRH)) |= (6 << ((9 - 8) * 4)); // PA9 AF6
    *((volatile uint32_t*) (GPIOA + GPIO_AFRH)) |= (6 << ((10 - 8) * 4)); // PA10 AF6
    *((volatile uint32_t*) (GPIOA + GPIO_AFRH)) |= (6 << ((11 - 8) * 4)); // PA11 AF6
    *((volatile uint32_t*) (GPIOA + GPIO_AFRH)) |= (6 << ((12 - 8) * 4)); // PA12 AF6
    *((volatile uint32_t*) (GPIOB + GPIO_AFRL)) |= (6 << ((1 - 0) * 4)); // PB1 AF6
}
