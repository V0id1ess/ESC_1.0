#ifndef PI_H
#define PI_H

class PIController {
    public:
        PIController(float kpGain, float kiGain);
        float compute(float setpoint, float measured_value, float dt);
};

#endif