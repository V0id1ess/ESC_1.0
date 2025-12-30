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

void configurePWMs() {
    // Configure TIM1_CH1 & TIM1_CH1N for PWM
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) &= ~(0b11 << 0); // CC1 config as output
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 1); // CC1P active high
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 3); // CC1NP active high
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (0b110 << 4); // OC1M PWM mode 1
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (1 << 3); // OC1PE Enable Preload

    // Configure TIM1_CH2 & TIM1_CH2N for PWM
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) &= ~(0b11 << 8); // CC2 config as output
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 5); // CC2P active high
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 7); // CC2NP active high
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (0b110 << 12); // OC2M PWM mode 1
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (1 << 15); // OC2PE Enable Preload

    // Configure TIM1_CH3 & TIM1_CH3N for PWM
    *((volatile uint32_t*) (TIM1 + TIM_CCMR2)) &= ~(0b11 << 0); // CC3 config as output
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 9); // CC3P active high
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 11); // CC3NP active high
    *((volatile uint32_t*) (TIM1 + TIM_CCMR2)) |= (0b110 << 4); // OC3M PWM mode 1
    *((volatile uint32_t*) (TIM1 + TIM_CCMR2)) |= (1 << 3); // OC3PE Enable Preload
    
    // f_pwm = f_sysck / (2 * (psc + 1) * (arr + 1))
    // 72MHz / (2 * (0 + 1) * (1439 + 1)) = 25kHz
    *((volatile uint32_t*) (TIM1 + TIM_PSC)) = 0; // Prescaler = 0
    *((volatile uint32_t*) (TIM1 + TIM_ARR)) = 1439; // Auto-reload = 1439; // Period = 40us

    // Enable Preloads
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (1 << 3); // CH1 OC1PE Enable Preload
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (1 << 11); // CH2 OC2PE Enable Preload
    *((volatile uint32_t*) (TIM1 + TIM_CCMR2)) |= (1 << 3); // CH3 OC3PE Enable Preload
    *((volatile uint32_t*) (TIM1 + TIM_CR1)) |= (1 << 7); // Auto-Reload Preload Enable

    *((volatile uint32_t*) (TIM1 + TIM_CR1)) |= (0b01 << 5); // Enable Center-Aligned Mode 1

    // Enable capture compares
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 0); // CC1PE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 2); // CC1NE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 4); // CC2PE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 6); // CC2NE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 8); // CC3PE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1 << 10); // CC3NE output enable

    *((volatile uint32_t*) (TIM1 + TIM_CR1)) |= (1 << 0); // Enable Counter
}

void setPWMDutyCycle(uint16_t dutyCycle) {
    // dutyCycle range: 0 - 4096
    // Map to 0 - 1439
    uint16_t mapped = (dutyCycle * 1439) / 4096;
    *((volatile uint32_t*) (TIM1 + TIM_CCR1)) = mapped; // Set Duty Cycle for CH1
    *((volatile uint32_t*) (TIM1 + TIM_CCR2)) = mapped; // Set Duty Cycle for CH2
    *((volatile uint32_t*) (TIM1 + TIM_CCR3)) = mapped; // Set Duty Cycle for CH3
}


