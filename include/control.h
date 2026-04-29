#ifndef CONTROL_H
#define CONTROL_H

#include <common.h>
#include <foc.h>
#include <profiler.h>


// Global Motor Motion State
enum ControlState {ALIGN, LOW_BEMF, RUNNING};
ControlState motorState = ALIGN;

// Global Variables for Phase Currents & Voltages
extern Vector3D I;
extern Vector3D V;
extern Vector3D I_offset;

extern Vector2D Istat, Vstat; // Stationary Reference Frame (alpha-beta)
extern Vector2D Irot, Vrot; // Rotating Reference Frame (d-q)
extern Vector3D signals; // SVPWM Duty Cycle Signals for 3 Phases

// Motion Variables
extern float throttle; // 0-100% Throttle Command
extern float jerk; // Current jerk (testing use only)
extern Profile profile; // Motion Profile for Velocity and Acceleration
extern Profile targetProfile; // Target Motion Profile for Velocity and Acceleration

extern void setDutyCycles(Vector3D voltages);
extern float ADCToCurrent(uint32_t adc);
extern float ADCToVoltage(uint32_t adc);
extern void FOC_update();

#endif