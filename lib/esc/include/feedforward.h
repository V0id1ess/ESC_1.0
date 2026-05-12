#ifndef FEEDFORWARD_H
#define FEEDFORWARD_H
#pragma once

#include <control.h>

// Feedforward Control
class Feedforward {
    public:
        /** @brief Constructor for the Feedforward class
         *  @param kV The velocity gain (Amps per (meter/second))
         *  @param kA The acceleration gain (Amps per (meter/second^2))
         */
        Feedforward(float kV, float kA) : kV(kV), kA(kA) {}

        /** @brief Calculate the feedforward control signal
         *  @param profile The current profile
         *  @return The feedforward control signal (Amps)
         */
        float compute(Profile profile) {
            return (kV * profile.velocity) + (kA * profile.acceleration);
        }
    private:
        float kV; // Velocity gain
        float kA; // Acceleration gain
};

#endif