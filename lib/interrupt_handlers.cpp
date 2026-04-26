#include <stm32f303cbt6.h>
#include <constants.h>
#include <control.h>
#include <common.h>

extern "C" void ADC1_2_IRQHandler(void) {
    // Phase Voltage Feedback
    if (*((volatile uint32_t*) (ADC2 + ADC_ISR)) & (1U << 6)) { // JEOS Flag
        *((volatile uint32_t*) (ADC2 + ADC_ISR)) |= (1U << 6); // Clear JEOC Flag

        V.x = ADCToVoltage(*((volatile uint32_t*) (ADC2 + ADC_JDR1)));
        V.y = ADCToVoltage(*((volatile uint32_t*) (ADC2 + ADC_JDR2)));
        V.z = ADCToVoltage(*((volatile uint32_t*) (ADC2 + ADC_JDR3)));
    }
}

extern "C" void ADC4_IRQHandler(void) {
    // Phase Current Feedback
    if (*((volatile uint32_t*) (ADC4 + ADC_ISR)) & (1U << 6)) { // JEOS Flag
        *((volatile uint32_t*) (ADC4 + ADC_ISR)) |= (1U << 6); // Clear JEOC Flag

        I.x = ADCToCurrent(*((volatile uint32_t*) (ADC4 + ADC_JDR1))) - I_offset.x;
        I.z = ADCToCurrent(*((volatile uint32_t*) (ADC4 + ADC_JDR2))) - I_offset.z;

        // Kirchoff's Law
        I.y = -(I.x + I.z);
    }
}