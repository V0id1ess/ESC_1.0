#include <common.h>
#include <stm32f303cbt6.h>
#include <constants.h>
#include <foc.h>
#include <smo.h>
#include <control.h>
#include <pi.h>

float align_time = 0.0f; // Time spent in ALIGN state
float angle; // Electrical angle

PIController IdController(Id_p, Id_i);
PIController IqController(Iq_p, Iq_i);

float prevTime;

void clamp(uint16_t &value, uint16_t min, uint16_t max) {
    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
}

float clamp(float &value, float min, float max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    return value;
}

void setDutyCycles(Vector3D voltages) {
    // Voltage Range: -MAX_VOLTAGE - +MAX_VOLTAGE
    // Map to 0 - PWM_ARR
    float center = static_cast<float>(PWM_ARR) / 2.0f; // Mid-point of PWM_ARR scale
    float mul = static_cast<float>(PWM_ARR) / MAX_VOLTAGE;

    uint16_t ch1 = static_cast<uint16_t>((voltages.x * mul) + center);
    uint16_t ch2 = static_cast<uint16_t>((voltages.y * mul) + center);
    uint16_t ch3 = static_cast<uint16_t>((voltages.z * mul) + center);

    clamp(ch1, 0, PWM_ARR);
    clamp(ch2, 0, PWM_ARR);
    clamp(ch3, 0, PWM_ARR);

    *((volatile uint32_t*) (TIM1 + TIM_CCR1)) = ch1; // Set Duty Cycle for CH1
    *((volatile uint32_t*) (TIM1 + TIM_CCR2)) = ch2; // Set Duty Cycle for CH2
    *((volatile uint32_t*) (TIM1 + TIM_CCR3)) = ch3; // Set Duty Cycle for CH3
}

float ADCToCurrent(uint32_t adc) {
    // ADC: 3.3V VREF, 12-bit resolution
    // Elec: 0.0006 ohm Shunt, 100x OpAmp
    // out / 4096 * 3.3V = amps * shunt
    return (static_cast<float>(adc) * 0.01342773437f);
}

float ADCToVoltage(uint32_t adc) {
    // ADC: 3.3V VREF, 12-bit resolution
    // Elec: Voltage Divider R2 = 5k, R1 = 36k
    // out / 4096 * 3.3V = volts * 5kohms / 41kohms
    return (static_cast<float>(adc) / 151.367331855f);
}

void FOC_update() {
    clarke(I, Istat);

    // Position Generator (SMO)

    switch(motorState) {
        case ALIGN:
            angle = 0.0f; // Force rotor to a known position
            throttle = 0.5f; // Apply moderate throttle to encourage alignment
            align_time += DT;
            if (align_time >= 0.500f) { // After 500 ms, transition to STARTING
                motorState = LOW_BEMF;
            }

            break;
        
        case LOW_BEMF:
            incrementVirtualAngle(1.0f); // Slowly ramp up virtual angle to encourage movement
            angle = getAngle();
            break;
        
        case RUNNING:
            angle = getAngle();
            break;
    } 

    park(Istat, angle, Irot);

    float Iq_setpoint = (clamp(throttle, 0.0f, 100.0f) / 100.0f * MAX_CURRENT);

    Vrot.x = IdController.compute(0.0f, Irot.x, DT); // V_sd
    Vrot.y = IqController.compute(Iq_setpoint, Irot.y, DT); // V_sq

    park_inverse(Vrot, angle, Vstat);

    // SVPWM
    clarke_inverse(Vstat, signals);

    setDutyCycles(signals);
}
