#include <common.h>
#include <stm32f303cbt6.h>
#include <constants.h>

uint16_t signalInput, phaseAFeedBack, phaseBFeedBack, phaseCFeedBack, phaseACurrent, phaseBCurrent, phaseCCurrent;

void clamp(uint16_t &value, uint16_t min, uint16_t max) {
    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
}

void setCH1PWMDutyCycle(uint16_t dutyCycle) {
    // dutyCycle range: 0 - 4096
    // Map to 0 - PWM_ARR
    uint16_t mapped = (dutyCycle * PWM_ARR) / 4096;
    clamp(mapped, 0, PWM_ARR);
    *((volatile uint32_t*) (TIM1 + TIM_CCR1)) = mapped; // Set Duty Cycle for CH1
}

void setCH2PWMDutyCycle(uint16_t dutyCycle) {
    // dutyCycle range: 0 - 4096
    // Map to 0 - 1439
    uint16_t mapped = (dutyCycle * PWM_ARR) / 4096;
    clamp(mapped, 0, PWM_ARR);
    *((volatile uint32_t*) (TIM1 + TIM_CCR2)) = mapped; // Set Duty Cycle for CH2
}

void setCH3PWMDutyCycle(uint16_t dutyCycle) {
    // dutyCycle range: 0 - 4096
    // Map to 0 - 1439
    uint16_t mapped = (dutyCycle * PWM_ARR) / 4096;
    clamp(mapped, 0, PWM_ARR);
    *((volatile uint32_t*) (TIM1 + TIM_CCR3)) = mapped; // Set Duty Cycle for CH3
}

long getNanoTime() {
    return 0; // Placeholder implementation
}

void updateInput() {
    *((volatile uint32_t*) (ADC1 + ADC_CR)) |= (1 << 2); // Start ADC1 Conversion
    while (!(*((volatile uint32_t*) (ADC1 + ADC_ISR)) & (1 << 2))); // ADC1 EOC

    signalInput = *((volatile uint32_t*) (ADC1 + ADC_DR)); // ADC1 Data CH1
}

void updateFeedback() {
    *((volatile uint32_t*) (ADC2 + ADC_CR)) |= (1 << 2); // Start ADC2 Conversion
    while (!(*((volatile uint32_t*) (ADC2 + ADC_ISR)) & (1 << 2))); // ADC2 EOC
    
    phaseAFeedBack = *((volatile uint32_t*) (ADC2 + ADC_DR)); // ADC2 Data CH1
    phaseBFeedBack = *((volatile uint32_t*) (ADC2 + ADC_DR + 4)); // ADC2 Data CH2
    phaseCFeedBack = *((volatile uint32_t*) (ADC2 + ADC_DR + 8)); // ADC2 Data CH3
}

void updateCurrentSense() {
    *((volatile uint32_t*) (ADC4 + ADC_CR)) |= (1 << 2); // Start ADC4 Conversion
    while (!(*((volatile uint32_t*) (ADC4 + ADC_ISR)) & (1 << 2))); // ADC4 EOC

    phaseACurrent = *((volatile uint32_t*) (ADC4 + ADC_DR + 8)); // ADC4 Data CH3
    phaseBCurrent = *((volatile uint32_t*) (ADC4 + ADC_DR + 16)); // ADC4 Data CH5
}

double getAFeedBack() {
    return phaseAFeedBack;
}

double getBFeedBack() {
    return phaseBFeedBack;
}

double getCFeedBack() {
    return phaseCFeedBack;
}

double getACurrent() {
    return phaseACurrent;
}

double getBCurrent() {
    return phaseBCurrent;
}

double getSignal() {
    return signalInput;
}