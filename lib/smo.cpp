#include <stm32f303cbt6.h>
#include <constants.h>
#include <control.h>
#include <common.h>
#include <trig.h>

float virtual_angle = 0.0f; // Virtual electrical angle for sensorless control
float elec_angle = 0.0f; // Measured electrical angle from back-EMF
float mech_angle = 0.0f; // Mechanical angle of the rotor
float prev_elec_angle = 0.0f; // Previous electrical angle
float prev_mech_angle = 0.0f; // Previous mechanical angle
float f_elec; // Current electrical frequency
float prev_velocity = 0.0f;
float prev_acceleration = 0.0f;

Vector2D I_pred; // Predicted current for SMO
Vector2D back_emf; // Back-EMF voltages for stationary frame (alpha-beta)

void incrementVirtualAngle(float speed) {
    virtual_angle += speed * DT; // Increment virtual angle based on estimated speed
    if (virtual_angle >= 360.0f) {
        virtual_angle -= 360.0f; // Wrap around at 360 degrees
    }
}

float square(float x) {
    return x * x;
}

float getAngle() {
    // If BEMF is strong enough, use measured angle; otherwise, rely on virtual angle
    if (square(back_emf.x) + square(back_emf.y) >= square(MIN_BEMF)) {
        motorState = RUNNING;
        virtual_angle = elec_angle; // Sync virtual angle to measured angle when reliable
        return elec_angle;
    } else {
        motorState = LOW_BEMF;
        return virtual_angle;
    }
}

float signum(float x) {
    if (x > PHI) return 1.0f;
    else if (x < -PHI) return -1.0f;
    else return x / PHI;
}

void SMO_update() {
    // Update Back-EMF Estimates
    back_emf.x += GAIN * signum(I.y - I_pred.y) * DT;
    back_emf.y += GAIN * signum(I.x - I_pred.x) * DT;

    elec_angle = atan2f(back_emf.y, back_emf.x) + 1.57079632679f; // Offset by 90 degrees to align with rotor position

    // Apply First-Order IIR filter to smooth angle estimation accounting for phase lag
    elec_angle = prev_elec_angle + LOW_PASS_ALPHA * (elec_angle - prev_elec_angle) + atan2f(f_elec, f_elec_MAX);

    mech_angle = elec_angle * (2.0f / POLES); // Convert electrical angle to mechanical angle
    prev_mech_angle = prev_elec_angle * (2.0f / POLES); // Store previous mechanical angle for velocity estimation

    // Mechanical Velocity, Acceleration, and Jerk Estimation for Motion Profiling
    profile.velocity = (mech_angle - prev_mech_angle) / DT;
    profile.acceleration = (profile.velocity - prev_velocity) / DT;
    jerk = (profile.acceleration - prev_acceleration) / DT;

    prev_elec_angle = elec_angle;
    prev_velocity = profile.velocity;
    prev_acceleration = profile.acceleration;

    f_elec = profile.velocity * POLES * PI; // Convert mechanical velocity to electrical frequency

    // Create new current predictions based on previous measurements, applied voltages, and back-EMF estimates
    I_pred.x = CONST_F * I.x + CONST_K * (Vstat.x - back_emf.x);
    I_pred.y = CONST_F * I.y + CONST_K * (Vstat.y - back_emf.y);
}
