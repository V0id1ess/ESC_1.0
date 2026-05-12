#ifndef CONTROL_H
#define CONTROL_H
#pragma once

#include <common.h>
#include <profiler.h>
#include <foc.h>


// Global Motor Motion State
enum ControlState {ALIGN, LOW_BEMF, RUNNING};
extern ControlState motorState;

// Global Variables for Phase Currents & Voltages
extern Vector3D I;
extern Vector3D V;
extern Vector3D I_offset;

extern Vector2D Istat, Vstat; // Stationary Reference Frame (alpha-beta)
extern Vector2D Irot, Vrot; // Rotating Reference Frame (d-q)
extern Vector3D signals; // SVPWM Duty Cycle Signals for 3 Phases

// Motion Variables
extern float jerk; // Current jerk (testing use only)
extern float throttle; // 0-100% Throttle Command
extern Profile profile; // Motion Profile for Velocity and Acceleration
extern Profile targetProfile; // Target Motion Profile for Velocity and Acceleration

static const float center = static_cast<float>(PWM_ARR) / 2.0f; // Mid-point of PWM_ARR scale
static const float mul = static_cast<float>(PWM_ARR) / MAX_VOLTAGE;

inline void clamp(uint16_t &value, uint16_t min, uint16_t max) {
    if (value < min) {
        value = min;
    } else if (value > max) {
        value = max;
    }
}

inline float clamp(float &value, float min, float max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    return value;
}

inline void setDutyCycles(Vector3D voltages) {
    // Voltage Range: -MAX_VOLTAGE - +MAX_VOLTAGE
    // Map to 0 - PWM_ARR

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

inline float ADCToCurrent(uint32_t adc) {
    return (static_cast<float>(adc) * 0.01342773437f);
}

inline float ADCToVoltage(uint32_t adc) {
    return (static_cast<float>(adc) * 0.00660644531f);
}

void FOC_update();

#endif