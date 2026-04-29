#ifndef FEEDFORWARD_H
#define FEEDFORWARD_H

#include <profiler.h>

class Feedforward {
    public:
        Feedforward(float kV, float kA);
        float compute(Profile profile);
};

#endif