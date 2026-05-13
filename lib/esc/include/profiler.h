#ifndef PROFILER_H
#define PROFILER_H
#pragma once

#include <common.h>
#include <constants.h>

struct Profile {
    float velocity;
    float acceleration;
};

// Jerk-Limited Trapezoidal Motion Profiler for Smooth Acceleration and Deceleration
class Profiler {
    public:
        /**
         * @brief Motion Profiler Constructor
         * @param kvGain Velocity gain (relates current to velocity)
         * @param kaGain Acceleration gain (relates current to acceleration)
         * @param j_max Maximum jerk (rate of change of acceleration)
         * Note: kaGain and kv should ideally be determined experimentally for the specific motor and load to achieve optimal performance.
         */
        Profiler(float kvGain, float kaGain, float j_max)
            : kv(kvGain), ka(kaGain), max_j(j_max) {
            inv_ka_headroom = (1.0f / ka) * 0.95f;
            inv_kv_headroom = (1.0f / kv) * 0.95f;
            inv_max_j = 1.0f / j_max;
        };

        /**
         * @brief Motion Profiling Compute Function using a Jerk-Limited Trapezoidal Profile
         * @param setpoint Desired velocity setpoint
         * @param profile Current motion profile containing velocity and acceleration
         * @param dt Time step for the next update
         */
        void compute(float throttle, Profile profile, Profile &targetProfile, float dt) {
            float abs_v = fabsf(profile.velocity);
            float abs_a = fabsf(profile.acceleration);

            float a_max = (MAX_CURRENT - kv * abs_v) * inv_ka_headroom; // Maximum acceleration based on current velocity
            float v_max = (MAX_CURRENT - ka * abs_a) * inv_kv_headroom; // Maximum velocity based on current acceleration

            float setpoint = throttle * v_max; // Desired velocity based on throttle input
            float error = setpoint - profile.velocity; // Velocity error
            
            float v_down = (0.5f * profile.acceleration * profile.acceleration) * inv_max_j; // Velocity taken to decelerate to 0 jerk
            
            float error_sign = (error > 0.0f) ? 1.0f : -1.0f;
            float jerk;

            // Final "Lock" onto the setpoint
            if (fabsf(error) < 0.1f && abs_a < 0.01f) {
                targetProfile.acceleration = 0.0f;
                targetProfile.velocity = setpoint;
                return;
            }

            if (fabsf(error) <= fabsf(v_down) && error * profile.velocity > 0) {
                // Deccellarating
                jerk = error_sign * -max_j;
            } else if (abs_a < a_max) {
                // Accelerating
                jerk = error_sign * max_j;
            } else {
                // Cruise
                jerk = 0.0f;
            }

            // Clamp outputs to maximums
            
            float a_next = profile.acceleration + jerk * dt;
            targetProfile.acceleration = fmaxf(fminf(a_next, a_max), -a_max);

            float v_next = profile.velocity + targetProfile.acceleration * dt;
            targetProfile.velocity = fmaxf(fminf(v_next, v_max), -v_max);
        }
    
    private:
    float kv, ka, max_j;
        float inv_ka_headroom, inv_kv_headroom, inv_max_j;

        inline float fabsf(float x) {
            return (x < 0.0f) ? -x : x;
        }

        inline float fmaxf(float a, float b) {
            return (a > b) ? a : b;
        }

        inline float fminf(float a, float b) {
            return (a < b) ? a : b;
        }
};

#endif