#include <esc.h>

/* PWM Pins
* PA8 - TIM1_CH1 - Motor Phase A High-Side
* PA9 - TIM1_CH2 - Motor Phase B High-Side
* PA10 - TIM1_CH3 - Motor Phase C High-Side
* PA11 - TIM1_CH1N - Motor Phase A Low-Side
* PA12 - TIM1_CH2N - Motor Phase B Low-Side
* PB1 - TIM1_CH3N - Motor Phase C Low-Side
*/

/* ADC Pins
* PA4 - ADC2_CH1 - Motor Phase A Feedback
* PA5 - ADC2_CH2 - Motor Phase B Feedback
* PA6 - ADC2_CH3 - Motor Phase C Feedback
* PB12 - ADC4_CH3 - Motor Phase A Current Sense
* PB14 - ADC4_CH5 - Motor Phase C Current Sense
*/

/* Main-Board Connections
* PA3 - ESC DShot Line
*/

void enableSystemClock() {
    // Configure main system clock
    *((volatile uint32_t*) (RCC + RCC_CR)) |= (1U << 0); // HSI ON
    while (! (*((volatile uint32_t*) (RCC + RCC_CR)) & (1U << 1))); // Wait for HSI to be ready
    
    *((volatile uint32_t*) (FLASH + 0b00)) |= (0b010 << 0); // Flash Latency = 2 WS (64MHz)

    *((volatile uint32_t*) (RCC + RCC_CFGR)) &= ~(1U << 16); // PLL source = HSI/2 (4MHz)
    *((volatile uint32_t*) (RCC + RCC_CFGR)) |= (0b1110 << 18); // PLL input clock x 9 (4MHz * 16 = 64MHz)

    *((volatile uint32_t*) (RCC + RCC_CR)) |= (1U << 24); // PLL ON 
    while (!(*((volatile uint32_t*) (RCC + RCC_CR)) & (1U << 25))); // Wait for PLL to be ready

    *((volatile uint32_t*) (RCC + RCC_CFGR)) |= (0b10 << 0); // SYSCLK Source = PLL
    while ((*((volatile uint32_t*) (RCC + RCC_CFGR)) & (0b11 << 2)) != (0b10 << 2)); // Wait for SYSCLK source to be switched

    *((volatile uint32_t*) (RCC + RCC_CFGR2)) &= ~(0b11111 << 0); // Clear bits for ADC12 prescaler
    *((volatile uint32_t*) (RCC + RCC_CFGR2)) |= (0b10000 << 0);

    // *** CLOCK IS NOW 64MHz ***
}

void configureClocks() {
    // Configure peripheral clocks
    *((volatile uint32_t*) (RCC + RCC_CFGR)) &= ~(0b1111 << 4); // No Prescaler on AHB (64MHz)
    *((volatile uint32_t*) (RCC + RCC_CFGR)) |= (0b100 << 11); // APB Low-Speed divided by 2 (32MHz)
    *((volatile uint32_t*) (RCC + RCC_CFGR)) &= ~(0b111 << 8); // APB High-Speed No Prescale (64MHz)

    *((volatile uint32_t*) (RCC + RCC_AHBENR)) |= (1U << 17); // Enable GPIOA Clock
    *((volatile uint32_t*) (RCC + RCC_AHBENR)) |= (1U << 18); // Enable GPIOB Clock
    *((volatile uint32_t*) (RCC + RCC_APB2ENR)) |= (1U << 11); // Enable TIM1 Clock
    *((volatile uint32_t*) (RCC + RCC_APB2ENR)) |= (1U << 14); // Enable USART1 Clock
    *((volatile uint32_t*) (RCC + RCC_AHBENR)) |= (1U << 28); // ADC12EN
    *((volatile uint32_t*) (RCC + RCC_AHBENR)) |= (1U << 29); // ADC34EN
    for(volatile int i = 0; i < 1000; i++) __asm__("nop");
}

void enableFPU() {
    volatile uint32_t *cpacr = (volatile uint32_t *)0xE000ED88;
    *cpacr |= ((3U << 10 * 2) | (3U << 11 * 2)); // Set bits 20-23

    __asm volatile ("dsb");
    __asm volatile ("isb");

    volatile uint32_t *fpccr = (volatile uint32_t *)0xE000EF34;
    *fpccr |= (1U << 31) | (1U << 30);
}

void LEDStartupSequence() {
    *((volatile uint32_t*) (GPIOA + GPIO_BRR)) |= (1U << 0); // PA0 LOW
    *((volatile uint32_t*) (GPIOA + GPIO_BSRR)) |= (1U << 1); // PA1 HIGH

    for (volatile unsigned long i = 0; i < (unsigned) (SYSCLK/13 / 2); i++) { // ~0.5s delay at 64MHz
        __asm__ volatile ("nop");
    }
    *((volatile uint32_t*) (GPIOA + GPIO_BSRR)) |= (1U << 0); // PA0 HIGH
    *((volatile uint32_t*) (GPIOA + GPIO_BRR)) |= (1U << 1); // PA1 LOW

    for (volatile unsigned long i = 0; i < (unsigned) (SYSCLK/13 / 8); i++) { // ~0.125s delay at 64MHz
        __asm__ volatile ("nop");
    }
    *((volatile uint32_t*) (GPIOA + GPIO_BRR)) |= (1U << 0); // PA0 LOW
    *((volatile uint32_t*) (GPIOA + GPIO_BSRR)) |= (1U << 1); // PA1 HIGH

    for (volatile unsigned long i = 0; i < (unsigned) (SYSCLK/13 / 4); i++) { // ~0.125s delay at 64MHz
        __asm__ volatile ("nop");
    }
    *((volatile uint32_t*) (GPIOA + GPIO_BRR)) |= (1U << 1); // PA1 LOW
}

void configureLEDs() {
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) &= ~(0b11 << 0); // PA0 as Output (LED)
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |=  (0b01 << 0);
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) &= ~(0b11 << 2); // PA1 as Output (LED)
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |=  (0b01 << 2);

    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << 0); // PA0 High Speed
    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << 2); // PA1 High Speed

    *((volatile uint32_t*) (GPIOA + GPIO_BSRR)) |= (1U << 0); // PA0 HIGH
    *((volatile uint32_t*) (GPIOA + GPIO_BSRR)) |= (1U << 1); // PA1 HIGH

    LEDStartupSequence();
}

void enablePWM() {
    // Alternate Function Mode
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b10 << (8 * 2)); // PA8 AF
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b10 << (9 * 2)); // PA9 AF
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b10 << (10 * 2)); // PA10 AF
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b10 << (11 * 2)); // PA11 AF
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b10 << (12 * 2)); // PA12 AF
    *((volatile uint32_t*) (GPIOB + GPIO_MODER)) |= (0b10 << (1 * 2)); // PB1 AF

    // High-Speed
    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << (8 * 2)); // PA8 High Speed
    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << (9 * 2)); // PA9 High Speed
    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << (10 * 2)); // PA10 High Speed
    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << (11 * 2)); // PA11 High Speed
    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << (12 * 2)); // PA12 High Speed
    *((volatile uint32_t*) (GPIOB + GPIO_OSPEEDR)) |= (0b11 << (1 * 2)); // PB1 High Speed

    // Pull-Down
    *((volatile uint32_t*) (GPIOA + GPIO_PUPDR)) |= (0b10 << (8 * 2)); // PA8 Pull-Down
    *((volatile uint32_t*) (GPIOA + GPIO_PUPDR)) |= (0b10 << (9 * 2)); // PA9 Pull-Down
    *((volatile uint32_t*) (GPIOA + GPIO_PUPDR)) |= (0b10 << (10 * 2)); // PA10 Pull-Down
    *((volatile uint32_t*) (GPIOA + GPIO_PUPDR)) |= (0b10 << (11 * 2)); // PA11 Pull-Down
    *((volatile uint32_t*) (GPIOA + GPIO_PUPDR)) |= (0b10 << (12 * 2)); // PA12 Pull-Down
    *((volatile uint32_t*) (GPIOB + GPIO_PUPDR)) |= (0b10 << (1 * 2)); // PB1 Pull-Down

    // PWM Alternate Function
    *((volatile uint32_t*) (GPIOA + GPIO_AFRH)) |= (6U << ((8 - 8) * 4)); // PA8 AF6
    *((volatile uint32_t*) (GPIOA + GPIO_AFRH)) |= (6U << ((9 - 8) * 4)); // PA9 AF6
    *((volatile uint32_t*) (GPIOA + GPIO_AFRH)) |= (6U << ((10 - 8) * 4)); // PA10 AF6
    *((volatile uint32_t*) (GPIOA + GPIO_AFRH)) |= (6U << ((11 - 8) * 4)); // PA11 AF6
    *((volatile uint32_t*) (GPIOA + GPIO_AFRH)) |= (6U << ((12 - 8) * 4)); // PA12 AF6
    *((volatile uint32_t*) (GPIOB + GPIO_AFRL)) |= (6U << ((1 - 0) * 4)); // PB1 AF6
}

void configurePWM() {
    // Configure TIM1_CH1 & TIM1_CH1N for PWM
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) &= ~(0b11 << 0); // CC1 config as output
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 1); // CC1P active high
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 3); // CC1NP active high
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (0b110 << 4); // OC1M PWM mode 1
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (1U << 3); // OC1PE Enable Preload

    // Configure TIM1_CH2 & TIM1_CH2N for PWM
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) &= ~(0b11 << 8); // CC2 config as output
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 5); // CC2P active high
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 7); // CC2NP active high
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (0b110 << 12); // OC2M PWM mode 1
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (1U << 15); // OC2PE Enable Preload

    // Configure TIM1_CH3 & TIM1_CH3N for PWM
    *((volatile uint32_t*) (TIM1 + TIM_CCMR2)) &= ~(0b11 << 0); // CC3 config as output
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 9); // CC3P active high
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 11); // CC3NP active high
    *((volatile uint32_t*) (TIM1 + TIM_CCMR2)) |= (0b110 << 4); // OC3M PWM mode 1
    *((volatile uint32_t*) (TIM1 + TIM_CCMR2)) |= (1U << 3); // OC3PE Enable Preload
    
    // f_pwm = f_sysck / ((psc + 1) * (arr + 1))
    // arr = f_sysck / (f_pwm * (psc + 1)) - 1
    *((volatile uint32_t*) (TIM1 + TIM_PSC)) = 0U; // Prescaler = 0
    *((volatile uint32_t*) (TIM1 + TIM_ARR)) = PWM_ARR; // Auto-reload = 1439; // Period = 40us

    // Enable Preloads
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (1U << 3); // CH1 OC1PE Enable Preload
    *((volatile uint32_t*) (TIM1 + TIM_CCMR1)) |= (1U << 11); // CH2 OC2PE Enable Preload
    *((volatile uint32_t*) (TIM1 + TIM_CCMR2)) |= (1U << 3); // CH3 OC3PE Enable Preload
    *((volatile uint32_t*) (TIM1 + TIM_CR1)) |= (1U << 7); // Auto-Reload Preload Enable

    *((volatile uint32_t*) (TIM1 + TIM_CR1)) |= (0b01 << 5); // Enable Center-Aligned Mode 1

    // Dead-time configuration (Current: 406.25ns. Part shows 42.4ns Turn-off Delay & Fall Time)
    *((volatile uint32_t*) (TIM1 + TIM_BDTR)) |= (26U << 0);

    // Enable capture compares
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 0); // CC1PE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 2); // CC1NE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 4); // CC2PE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 6); // CC2NE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 8); // CC3PE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 10); // CC3NE output enable

    // Set all PWM channel duty cycles to 0%
    *((volatile uint32_t*) (TIM1 + TIM_CCR1)) = 0; // Set Duty Cycle for CH1
    *((volatile uint32_t*) (TIM1 + TIM_CCR2)) = 0; // Set Duty Cycle for CH2
    *((volatile uint32_t*) (TIM1 + TIM_CCR3)) = 0; // Set Duty Cycle for CH3
    
    *((volatile uint32_t*) (TIM1 + TIM_CR2)) &= ~(0b111 << 4); // Clear MMS bits
    *((volatile uint32_t*) (TIM1 + TIM_CR2)) |=  (0b010 << 4); // Set MMS to 010 (Update)

    // Center-Aligned Mode & Preload Enable
    // CMS = 0b01 (Center-aligned mode 1, flags set on counting down)
    *((volatile uint32_t*) (TIM1 + TIM_CR1)) |= (0b01 << 5) | (1U << 7);

    // Dead-time and Main Output Enable (MOE)
    *((volatile uint32_t*) (TIM1 + TIM_BDTR)) |= (26U << 0); // Deadtime
    *((volatile uint32_t*) (TIM1 + TIM_BDTR)) |= (1U << 15); // MOE = 1

    *((volatile uint32_t*) (TIM1 + TIM_CR1)) |= (1U << 0); // Enable Counter
}

void configureADC() {

    // Configure GPIO Pins for Analog Mode
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b11 << (4 * 2)); // PA4
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b11 << (5 * 2)); // PA5
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b11 << (6 * 2)); // PA6
    *((volatile uint32_t*) (GPIOB + GPIO_MODER)) |= (0b11 << (12 * 2)); // PB12
    *((volatile uint32_t*) (GPIOB + GPIO_MODER)) |= (0b11 << (14 * 2)); // PB14

    // Set Synchronous Clock (AHB/1)
    *((volatile uint32_t*) ADC12_COMMON_CCR) = (0b01 << 16); 
    *((volatile uint32_t*) ADC34_COMMON_CCR) = (0b01 << 16);

    // Exit Deep Power Down
    *((volatile uint32_t*) (ADC2 + ADC_CR)) &= ~(1U << 29); 
    *((volatile uint32_t*) (ADC4 + ADC_CR)) &= ~(1U << 29);
    for(volatile uint32_t i=0; i<1000; i++) __asm__("nop");

    // Enable Voltage Regulator
    *((volatile uint32_t*) (ADC2 + ADC_CR)) |= (1U << 28);
    *((volatile uint32_t*) (ADC4 + ADC_CR)) |= (1U << 28);
    // Mandatory wait for Startup time (T_ADCVREG_STUP)
    for(volatile uint32_t i = 0; i < 1000; i++) __asm__("nop");

    // Calibration
    *((volatile uint32_t*) (ADC2 + ADC_CR)) |= (1U << 31);
    while (*((volatile uint32_t*) (ADC2 + ADC_CR)) & (1U << 31)); 
    *((volatile uint32_t*) (ADC4 + ADC_CR)) |= (1U << 31);
    while (*((volatile uint32_t*) (ADC4 + ADC_CR)) & (1U << 31));

    // Enable ADCs and wait for ARDY
    *((volatile uint32_t*) (ADC2 + ADC_CR)) |= (1U << 0); // ADEN = 1
    while(!(*((volatile uint32_t*) (ADC2 + ADC_ISR)) & (1U << 0))); 

    *((volatile uint32_t*) (ADC4 + ADC_CR)) |= (1U << 0); // ADEN = 1
    while(!(*((volatile uint32_t*) (ADC4 + ADC_ISR)) & (1U << 0)));

    *((volatile uint32_t*) (ADC4 + ADC_JSQR)) = (0b01 << 0) | (3U << 8) | (5U << 14);

    *((volatile uint32_t*) (ADC4 + ADC_ISR)) = (1U << 6);

    // Initial Offset Readings
    *((volatile uint32_t*) (ADC4 + ADC_CR)) |= (1U << 3); // JADSTART
    while (!(*((volatile uint32_t*) (ADC4 + ADC_ISR)) & (1U << 6))); // Wait for JEOS

    I_offset.x = *((volatile uint32_t*) (ADC4 + ADC_JDR1));
    I_offset.z = *((volatile uint32_t*) (ADC4 + ADC_JDR2));

    *((volatile uint32_t*) (ADC4 + ADC_ISR)) = (1U << 6); // Clear JEOS flag

    // ADC2 Injected Sequence Configuration
    // L=3 (0b10), Trigger=TIM1_TRGO (0b0000), Edge=Rising (0b01), Channels 1, 2, 3
    *((volatile uint32_t*) (ADC2 + ADC_JSQR)) = (0b10 << 0)   | // Length = 3
                                                (0b0000 << 2) | // JEXTSEL = TIM1_TRGO
                                                (0b01 << 6)   | // JEXTEN = Rising Edge
                                                (1U << 8)     | // JSQ1 = CH1
                                                (2U << 14)    | // JSQ2 = CH2
                                                (3U << 20);     // JSQ3 = CH3

    // ADC4 Injected Sequence Configuration
    // L=2 (0b01), Trigger=TIM1_TRGO (0b0000), Edge=Rising (0b01), Channels 3, 5
    *((volatile uint32_t*) (ADC4 + ADC_JSQR)) = (0b01 << 0)   | // Length = 2
                                                (0b0000 << 2) | // JEXTSEL = TIM1_TRGO
                                                (0b01 << 6)   | // JEXTEN = Rising Edge
                                                (3U << 8)     | // JSQ1 = CH3
                                                (5U << 14);     // JSQ2 = CH5

    // Trigger Confguration: Trigger on TIM1 Rising Edge Event (TRGO)
    *((volatile uint32_t*) (ADC2 + ADC_JSQR)) |= (0b0000 << 2);
    *((volatile uint32_t*) (ADC4 + ADC_JSQR)) |= (0b0000 << 2);
    *((volatile uint32_t*) (ADC2 + ADC_JSQR)) |= (1U << 6);
    *((volatile uint32_t*) (ADC4 + ADC_JSQR)) |= (1U << 6);

    // Enable Interrupts for End of Injected Sequence of Conversions
    *((volatile uint32_t*) (ADC2 + ADC_IER)) |= (1U << 6); // JEOSIE
    *((volatile uint32_t*) (ADC4 + ADC_IER)) |= (1U << 6);

    *((volatile uint32_t*) (ADC2 + ADC_CR)) |= (1U << 3); // JADSTART
    *((volatile uint32_t*) (ADC4 + ADC_CR)) |= (1U << 3); // JADSTART

    // Clear JEOS flags
    *((volatile uint32_t*) (ADC2 + ADC_ISR)) = (1U << 6);
    *((volatile uint32_t*) (ADC4 + ADC_ISR)) = (1U << 6);
}

void configureDShot() {
    // Configure PA3 as Alternate Function for DShot Output
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b10 << (3 * 2)); // PA3 AF
    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << (3 * 2)); // PA3 High Speed
    *((volatile uint32_t*) (GPIOA + GPIO_PUPDR)) |= (0b10 << (3 * 2)); // PA3 Pull-Down
}

void configureInterrupts() {
    // Clear any pending ADC interrupts
    *((volatile uint32_t*) (ADC2 + ADC_ISR)) = (1U << 6); 
    *((volatile uint32_t*) (ADC4 + ADC_ISR)) = (1U << 6);

    // Clear pending EXTI interrupts
    NVIC_ClearPendingIRQ(ADC1_2_IRQn);
    NVIC_ClearPendingIRQ(ADC4_IRQn);

    // Set ADC interrupts to highest priority
    NVIC_SetPriority(ADC1_2_IRQn, 0);
    NVIC_SetPriority(ADC4_IRQn, 0);

    // Enable ADC Interrupts
    NVIC_EnableIRQ(ADC1_2_IRQn);
    NVIC_EnableIRQ(ADC4_IRQn);
}