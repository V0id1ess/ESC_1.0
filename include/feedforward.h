#ifndef FEEDFORWARD_H
#define FEEDFORWARD_H

#include <profiler.h>

class Feedforward {
    public:
        Feedforward(float kV, float kA);
        float calculate(Profile profile);
};

#endif