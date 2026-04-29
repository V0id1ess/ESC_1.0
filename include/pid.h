#ifndef PID_H
#define PID_H

class PIDController {
    public:
        PIDController(float kpGain, float kiGain, float kdGain);
        float compute(float setpoint, float measured_value, float dt);
};

#endif