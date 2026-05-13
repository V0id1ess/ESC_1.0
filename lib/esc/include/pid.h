#ifndef PID_H
#define PID_H
#pragma once

#include <constants.h>
#include <common.h>
#include <profiler.h>

class PIDController {
public:
    // Store pre-multiplied gains
    PIDController(float kpGain, float kiGain, float kdGain, float dt) {
        kp = kpGain;
        ki_dt = kiGain * dt;
        kd_inv_dt = kdGain / dt;
        integral = 0.0f;
        prev_error = 0.0f;
    }

    // Mark as inline to ensure it flattens into FOC_update
    inline float compute(float setpoint, float measured){
        float error = setpoint - measured;
        
        // Proportional
        float proportional = kp * error;
        
        // Integral with clamping
        integral += ki_dt * error;
        if (integral > MAX_VOLTAGE) integral = MAX_VOLTAGE;
        else if (integral < -MAX_VOLTAGE) integral = -MAX_VOLTAGE;

        // Derivative
        float derivative = kd_inv_dt * (error - prev_error);
        prev_error = error;

        float output = proportional + integral + derivative;

        if (output > MAX_VOLTAGE) return MAX_VOLTAGE;
        if (output < -MAX_VOLTAGE) return -MAX_VOLTAGE;
        return output;
    }

private:
    float kp, ki_dt, kd_inv_dt;
    float integral, prev_error;
};

#endif