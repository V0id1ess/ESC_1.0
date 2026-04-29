#ifndef PROFILER_H
#define PROFILER_H

struct Profile {
    float velocity;
    float acceleration;
};

class Profiler {
    public:
        Profiler(float kvGain, float kaGain, float j_max, float max_voltage);
        void compute(float setpoint, Profile &profile, float dt);
};

#endif