#include <stm32f303cbt6.h>
#include <constants.h>
#include <control.h>
#include <common.h>
#include <trig.h>

float virtual_angle = 0.0f; // Virtual angle for sensorless control
float measured_angle = 0.0f; // Measured angle from back-EMF
float prev_angle = 0.0f;

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
        virtual_angle = measured_angle; // Sync virtual angle to measured angle when reliable
        return measured_angle;
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

    measured_angle = atan2f(back_emf.y, back_emf.x) + 1.57079632679f; // Offset by 90 degrees to align with rotor position

    velocity = (measured_angle - prev_angle) / DT * 2.0f / POLES;

    prev_angle = measured_angle;

    // Create new current predictions based on previous measurements, applied voltages, and back-EMF estimates
    I_pred.x = CONST_F * I.x + CONST_K * (Vstat.x - back_emf.x);
    I_pred.y = CONST_F * I.y + CONST_K * (Vstat.y - back_emf.y);
}
