#include <common.h>
#include <profiler.h>
#include <constants.h>

float abs(float x) {
    return (x < 0.0f) ? -x : x;
}

float sign(float x) {
    return (x < 0.0f) ? -1.0f: 1.0f;
}

// Jerk-Limited Trapezoidal Motion Profiler for Smooth Acceleration and Deceleration
class Profiler {
    public:
        /**
         * @brief Motion Profiler Constructor
         * @param kvGain Velocity gain (relates voltage to velocity)
         * @param kaGain Acceleration gain (relates voltage to acceleration)
         * @param j_max Maximum jerk (rate of change of acceleration)
         * @param max_voltage Maximum voltage that can be applied to the motor (used to calculate max acceleration at different speeds)
         * Note: kaGain and kv should ideally be determined experimentally for the specific motor and load to achieve optimal performance.
         */
        Profiler(float kvGain, float kaGain, float j_max, float max_voltage)
            :kv(kvGain), one_over_kv(1.0f/kv), ka(kaGain), one_over_ka(1.0f/ka), max_j(j_max), one_over_max_j(1.0f/j_max) {};

        /**
         * @brief Motion Profiling Compute Function using a Jerk-Limited Trapezoidal Profile
         * @param setpoint Desired velocity setpoint
         * @param profile Current motion profile containing velocity and acceleration
         * @param dt Time step for the next update
         */
        void compute(float setpoint, Profile &profile, float dt) {
            float error = setpoint - profile.velocity; // Velocity error
            
            float v_down = 0.5f * (profile.acceleration * profile.acceleration) * one_over_max_j; // Velocity taken to decelerate to 0 jerk

            float a_max = (MAX_VOLTAGE - kv * abs(profile.velocity)) * one_over_ka; // Maximum acceleration based on current velocity
            a_max *= 0.85f; // Provide headroom for PID

            float v_max = (MAX_VOLTAGE - ka * abs(profile.acceleration)) * one_over_kv; // Maximum velocity based on current acceleration

            float jerk = sign(error);

            // Final "Lock" onto the setpoint
            if (abs(error) <= 0.1f && abs(profile.acceleration) < 0.01f) {
                profile.acceleration = 0;
                profile.velocity = setpoint;
                return;
            }

            if (abs(error) <= abs(v_down) && error * profile.velocity > 0) {
                // Deccellarating
                jerk *= -max_j;
            } else if (abs(profile.acceleration) < a_max) {
                // Accelerating
                jerk *= max_j;
            } else {
                // Cruise
                jerk = 0;
            }

            // Clamp outputs to maximums
            float a_next = profile.acceleration + jerk * dt;
            if (abs(a_next) > a_max) {
                profile.acceleration = sign(a_next) * a_max;
            } else {
                profile.acceleration = a_next;
            }

            float v_next = profile.velocity + profile.acceleration * dt;
            if (abs(v_next) > v_max) {
                profile.velocity = sign(v_next) * v_max;
            } else {
                profile.velocity = v_next;
            }
        }
    
    private:
        float kv;
        float one_over_kv;
        float ka;
        float one_over_ka;
        float max_a;
        float max_j;
        float one_over_max_j;
};