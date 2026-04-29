#ifndef PROFILER_H
#define PROFILER_H

struct Profile {
    float velocity;
    float acceleration;
};

class Profiler {
    public:
        Profiler(float kvGain, float kaGain, float j_max);
        void compute(float setpoint, Profile profiler, Profile &targetProfile, float dt);
};

#endif