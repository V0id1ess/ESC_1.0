#include <pid.h>
#include <state.h>
#include "control.h"
#include "common.h"

#define STEPS_PER_REVOLUTION 6
#define PI 3.14159265358979323846

double time;
double velocity = 0.0;
double targetVelocity = 0.0;
double position = 0.0;
uint16_t signal = 0;
uint8_t step;

State A = State(getAFeedBack, getACurrent);
State B = State(getBFeedBack, getBCurrent);
State C = State(getCFeedBack, getCCurrent);

PID velocityController(0.0, 0.0, 0.0); // Placeholder gains

void incrementStep() {
    step++;
    if (step >= STEPS_PER_REVOLUTION) {
        step = 0;
    }
}

void decrementStep() {
    step--;
}

void nextStep() {
    if (step == 0 || 3) {
        while (!A.phaseChange());
    } else if (step == 1 || 4) {
        while (!B.phaseChange());
    } else if (step == 2 || 5) {
        while (!C.phaseChange());
    }

    double dt = (getNanoTime() - time) / 1.0e9; // Convert to seconds

    velocity = (2 * PI) / (STEPS_PER_REVOLUTION * dt); // rad/s

    signal = (uint8_t) velocityController.compute(velocity, targetVelocity, dt); // Placeholder setpoint 0.0

    time = getNanoTime();
    
    incrementStep();
}

void setTargetVelocity(double target) {
    targetVelocity = target;
}