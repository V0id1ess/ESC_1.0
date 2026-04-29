#include <profiler.h>
#include <control.h>

// Feedforward Control
class Feedforward {
    public:
        /** @brief Constructor for the Feedforward class
         *  @param kV The velocity gain
         *  @param kA The acceleration gain
         */
        Feedforward(float kV, float kA) : kV(kV), kA(kA) {}

        /** @brief Calculate the feedforward control signal
         *  @param profile The current profile
         *  @return The feedforward control signal
         */
        float calculate(Profile profile) {
            return (kV * profile.velocity) + (kA * profile.acceleration);
        }
    private:
        float kV; // Velocity gain
        float kA; // Acceleration gain
};