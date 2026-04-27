#ifndef CONTROL_H
#define CONTROL_H
#include <common.h>
#include <foc.h>


// Global Motor Motion State
enum ControlState {ALIGN, LOW_BEMF, RUNNING};
ControlState motorState = ALIGN;

// Global Variables for Phase Currents & Voltages
extern Vector3D I;
extern Vector3D V;
extern Vector3D I_offset;

Vector2D Istat, Vstat; // Stationary Reference Frame (alpha-beta)
Vector2D Irot, Vrot; // Rotating Reference Frame (d-q)
Vector3D signals; // SVPWM Duty Cycle Signals for 3 Phases

// Throttle reference (0 - 100)
extern float throttle = 0.0;

extern void setDutyCycles(Vector3D voltages);
extern float ADCToCurrent(uint32_t adc);
extern float ADCToVoltage(uint32_t adc);
extern void FOC_update();

#endif