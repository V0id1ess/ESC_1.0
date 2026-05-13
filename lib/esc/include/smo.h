#ifndef SMO_H
#define SMO_H
#pragma once
#include <common.h>

static float virtual_angle = 0.0f; // Virtual electrical angle for sensorless control
static float elec_angle = 0.0f; // Measured electrical angle from back-EMF
static float mech_angle = 0.0f; // Mechanical angle of the rotor
static float prev_elec_angle = 0.0f; // Previous electrical angle
static float prev_mech_angle = 0.0f; // Previous mechanical angle
static float f_elec; // Current electrical frequency
static float prev_velocity = 0.0f;
static float prev_acceleration = 0.0f;


#define PHI_INV 1.0f / PHI; // Precompute inverse of deadband threshold for signum function
#define ELEC_TO_MECH(angle) ((angle) * (2.0f / POLES)) // Convert electrical angle to mechanical angle

static Vector2D I_pred; // Predicted current for SMO
static Vector2D back_emf; // Back-EMF voltages for stationary frame (alpha-beta)

inline void incrementVirtualAngle(float speed) {
    virtual_angle += speed * DT; // Increment virtual angle based on estimated speed
}

inline float square(float x) {
    return x * x;
}

inline float getAngle() {
    // Only perform the square on the changing variables
    float bemf_mag_sq = (back_emf.x * back_emf.x) + (back_emf.y * back_emf.y);
    
    if (bemf_mag_sq > MIN_BEMF * MIN_BEMF) {
        motorState = RUNNING;
        virtual_angle = elec_angle;
        return elec_angle;
    } 
    
    motorState = LOW_BEMF;
    return virtual_angle;
}

inline float signum(float x) {
    if (x > PHI) return 1.0f;
    else if (x < -PHI) return -1.0f;
    else return x * PHI_INV;
}

inline void SMO_update() {
    // Update Back-EMF Estimates
    back_emf.x += GAIN * signum(I.y - I_pred.y) * DT;
    back_emf.y += GAIN * signum(I.x - I_pred.x) * DT;

    // Apply First-Order IIR filter to smooth angle estimation accounting for phase lag
    elec_angle = prev_elec_angle + LOW_PASS_ALPHA * (atan2f(back_emf.y, back_emf.x) + 1.57079632679f - prev_elec_angle) + atan2f(f_elec, f_elec_MAX);

    mech_angle = ELEC_TO_MECH(elec_angle); // Convert electrical angle to mechanical angle
    prev_mech_angle = ELEC_TO_MECH(prev_elec_angle); // Store previous mechanical angle for velocity estimation

    // Mechanical Velocity, Acceleration, and Jerk Estimation for Motion Profiling
    profile.velocity = (mech_angle - prev_mech_angle) * PWM;
    profile.acceleration = (profile.velocity - prev_velocity) * PWM;
    jerk = (profile.acceleration - prev_acceleration) * PWM;

    prev_elec_angle = elec_angle;
    prev_velocity = profile.velocity;
    prev_acceleration = profile.acceleration;

    f_elec = profile.velocity * POLES * PI; // Convert mechanical velocity to electrical frequency

    // Create new current predictions based on previous measurements, applied voltages, and back-EMF estimates
    I_pred.x = CONST_F * I.x + CONST_K * (Vstat.x - back_emf.x);
    I_pred.y = CONST_F * I.y + CONST_K * (Vstat.y - back_emf.y);
}

#endif