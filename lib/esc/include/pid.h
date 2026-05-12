#ifndef PID_H
#define PID_H
#pragma once

#include <constants.h>
#include <common.h>
#include <profiler.h>

class PIDController {
    public:
        PIDController(float kpGain, float kiGain, float kdGain)
            :kp(kpGain), ki(kiGain), kd(kdGain), integral(0.0), derivative(0.0) {}

        float compute(float setpoint, float measured_value, float dt) {
            float error = setpoint - measured_value;

            integral += error * dt;
            derivative = (error - previous_error) * PWM;
            previous_error = error;

            float output = kp * error + ki * integral + kd * derivative;

            if (output > MAX_VOLTAGE) {
                output = MAX_VOLTAGE;
                integral -= error * dt;
            } else if (output < -MAX_VOLTAGE) {
                output = -MAX_VOLTAGE;
                integral -= error * dt;
            }

            return output;
        }
    
    private:
        float kp;
        float ki;
        float kd;
        float previous_error;
        float integral;
        float derivative;
};

#endif