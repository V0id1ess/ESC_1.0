#include <stm32f303cbt6.h>
#include <constants.h>
#include <common.h>
#include <control.h>

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

void enableSystemClock() {
    // Configure main system clock
    *((volatile uint32_t*) (RCC + RCC_CR)) |= (1U << 0); // HSI ON
    while (! (*((volatile uint32_t*) (RCC + RCC_CR)) & (1U << 1))); // Wait for HSI to be ready

    *((volatile uint32_t*) (RCC + RCC_CFGR)) &= ~(1U << 16); // PLL source = HSI/2 (4MHz)
    *((volatile uint32_t*) (RCC + RCC_CFGR)) |= (0b1110 << 18); // PLL input clock x 9 (4MHz * 16 = 64MHz)

    *((volatile uint32_t*) (RCC + RCC_CFGR)) |= (0b10 << 0); // SYSCLK Source = PLL
    while (! (*((volatile uint32_t*) (RCC + RCC_CFGR)) & (0b10 << 2))); // Wait for SYSCLK source to be switched

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
    // *((volatile uint32_t*) (RCC + RCC_APB2ENR)) |= (1U << 14); // Enable USART1 Clock
    *((volatile uint32_t*) (RCC + RCC_AHBENR)) |= (1U << 28); // ADC12EN
    *((volatile uint32_t*) (RCC + RCC_AHBENR)) |= (1U << 29); // ADC34EN
}

void enableFPU() {
    // Enable CP10 & CP11 Coprocessors
    *((volatile uint32_t*) (FPU_CPACR)) |= (0xF << 20);

    // Ensure FPU is fully on
    __asm volatile ("dsb"); // Ensure Memory Operations are Completed
    __asm volatile ("isb"); // Flush Instruction Pipeline

    // Enable Lazy Stacking
    *((volatile uint32_t*) (FPU_FPCCR)) |= (1U << 31) | (1U << 30);
}

void configureLEDs() {
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b01 << 0); // PA0 as Output (LED)
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b01 << 2); // PA1 as Output (LED)

    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << 0); // PA0 High Speed
    *((volatile uint32_t*) (GPIOA + GPIO_OSPEEDR)) |= (0b11 << 2); // PA1 High Speed

    // Set PA0 to High
    *((volatile uint32_t*) (GPIOA + GPIO_BSRR)) |= (1U << 0); // PA0 High

    // Set PA1 to Low
    *((volatile uint32_t*) (GPIOA + GPIO_BRR)) |= (1U << 1); // PA1 Low
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

void configurePWMs() {
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

    // Dead-time configuration (406.25ns)
    *((volatile uint32_t*) (TIM1 + TIM_BDTR)) |= (26U << 0);

    // Enable capture compares
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 0); // CC1PE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 2); // CC1NE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 4); // CC2PE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 6); // CC2NE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 8); // CC3PE output enable
    *((volatile uint32_t*) (TIM1 + TIM_CCER)) &= ~(1U << 10); // CC3NE output enable

    // Set all PWM channel duty cycles to 0%
    setCH1PWMDutyCycle(0);
    setCH2PWMDutyCycle(0);
    setCH3PWMDutyCycle(0);

    *((volatile uint32_t*) (TIM1 + TIM_CR1)) |= (1U << 0); // Enable Counter
}

void configureADC() {

    // Enable Internal Regulator
    *((volatile uint32_t*) (ADC2 + ADC_CR)) &= ~(0b11 << 28); // Clear ADVREGEN bits
    *((volatile uint32_t*) (ADC2 + ADC_CR)) |= (1U << 28); // Set ADVREGEN to 01
    *((volatile uint32_t*) (ADC4 + ADC_CR)) &= ~(0b11 << 28); // Clear ADVREGEN bits
    *((volatile uint32_t*) (ADC4 + ADC_CR)) |= (1U << 28); // Set ADVREGEN to 01
    for(volatile uint16_t i = 0; i < 1000; i++);

    // ADC Calibration
    *((volatile uint32_t*) (ADC2 + ADC_CR)) |= (1U << 31); // Start calibration (ADCAL)
    while (*((volatile uint32_t*) (ADC2 + ADC_CR)) & (1U << 31)); // Wait for ADCAL to clear
    *((volatile uint32_t*) (ADC4 + ADC_CR)) |= (1U << 31); // Start calibration (ADCAL)
    while (*((volatile uint32_t*) (ADC4 + ADC_CR)) & (1U << 31)); // Wait for ADCAL to clear

    // Configure Analog
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b11 << (4 * 2)); // PA4 Analog
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b11 << (5 * 2)); // PA4 Analog
    *((volatile uint32_t*) (GPIOA + GPIO_MODER)) |= (0b11 << (6 * 2)); // PA6 Analog
    *((volatile uint32_t*) (GPIOB + GPIO_MODER)) |= (0b11 << (12 * 2)); // PB12 Analog
    *((volatile uint32_t*) (GPIOB + GPIO_MODER)) |= (0b11 << (14 * 2)); // PB14 Analog

    // Set ADC Resolutions
    *((volatile uint32_t*) (ADC2 + ADC_CFGR)) &= ~(0b111 << 3); // 12-bit resolution
    *((volatile uint32_t*) (ADC4 + ADC_CFGR)) &= ~(0b111 << 3); // 12-bit resolution
    
    // ADC2 Injection Channel Length = 3 conversions
    *((volatile uint32_t*) (ADC2 + ADC_JSQR)) &= ~(0b11 << 0);
    *((volatile uint32_t*) (ADC2 + ADC_JSQR)) |= (0b10 << 0);
     // ADC4 Inujection Channel Length = 2 conversions
    *((volatile uint32_t*) (ADC4 + ADC_JSQR)) &= ~(0b11 << 0);
    *((volatile uint32_t*) (ADC4 + ADC_JSQR)) |= (0b01 << 0);

    // Enable End of Injected Conversion Sequence Interrupt
    *((volatile uint32_t*) (ADC2 + ADC_IER)) |= (1U << 6);

    // Update Event as TIM1 Trigger Output
    *((volatile uint32_t*) (TIM1 + TIM_CR2)) |= (0b010 << 4);

    // Set JEXTSEL to trigger when PWM cycle starts
    *((volatile uint32_t*) (ADC2 + ADC_JSQR)) |= (0b01 << 6); // Enable Trigger on Rising Edge
    *((volatile uint32_t*) (ADC2 + ADC_JSQR)) |= (0b0000 << 2); // For clarity. TIM1_TRGO as Conversion Start Trigger

    *((volatile uint32_t*) (ADC4 + ADC_JSQR)) |= (0b01 << 6); // Enable Trigger on Rising Edge
    *((volatile uint32_t*) (ADC4 + ADC_JSQR)) |= (0b0000 << 2); // For clarity. TIM1_TRGO as Conversion Start Trigger

    // Set Injected Conversions Sequence
    *((volatile uint32_t*) (ADC2 + ADC_JSQR)) |= (1U << 8); // ADC2 1st conversion = CH1 (PA4)
    *((volatile uint32_t*) (ADC2 + ADC_JSQR)) |= (2U << 14); // ADC2 2nd conversion = CH2 (PA5)
    *((volatile uint32_t*) (ADC2 + ADC_JSQR)) |= (3U << 20); // ADC2 3rd conversion = CH3 (PA6)
    *((volatile uint32_t*) (ADC4 + ADC_JSQR)) |= (3U << 8); // ADC4 1st conversion = CH3 (PB12)
    *((volatile uint32_t*) (ADC4 + ADC_JSQR)) |= (5U << 14); // ADC4 2nd conversion = CH5 (PB14)

    // Enable ADCs
    *((volatile uint32_t*) (ADC2 + ADC_CR)) |= (1U << 0); // ADC2 Enable
    *((volatile uint32_t*) (ADC4 + ADC_CR)) |= (1U << 0); // ADC4 Enable
    while(!(*((volatile uint32_t*) (ADC2 + ADC_ISR)) & (1U << 0))); // Wait for ADRDY
    while(!(*((volatile uint32_t*) (ADC4 + ADC_ISR)) & (1U << 0))); // Wait for ADRDY

    // *** Zero-Offset Configuring ***
    *((volatile uint32_t*) (ADC4 + ADC_CR)) |= (1U << 3); // Start of Injected Conversion ADC4

    while (!(*((volatile uint32_t*) (ADC4 + ADC_ISR)) & (1U << 6))); // Wait for ADC4 Conversion
    
    // Add Offsets
    I_offset.x = ADCToCurrent(*((volatile uint32_t*) (ADC4 + ADC_JDR1)));
    I_offset.z = ADCToCurrent(*((volatile uint32_t*) (ADC4 + ADC_JDR2)));
}

void configureInterrupts() {
    // Enable ADC End of Injected Sequence of Conversions Interrupts
    *((volatile uint32_t*) (ADC2 + ADC_IER)) |= (1U << 6);
    *((volatile uint32_t*) (ADC4 + ADC_IER)) |= (1U << 6);

    // Set ADC interrupts to highest priority
    NVIC_SetPriority(ADC1_2_IRQn, 0);
    NVIC_SetPriority(ADC4_IRQn, 0);

    // Enable ADC Interrupts
    NVIC_EnableIRQ(ADC1_2_IRQn);
    NVIC_EnableIRQ(ADC4_IRQn);
}