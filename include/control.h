#ifndef CONTROL_H
#define CONTROL_H
#include <common.h>
#include <operations.h>

// Global Variables for Phase Currents & Voltages
extern Vector3D I;
extern Vector3D V;
extern Vector3D I_offset;

// Throttle reference (0 - 100)
extern float throttle = 0.0;

extern void setCH1PWMDutyCycle(uint16_t dutyCycle);
extern void setCH2PWMDutyCycle(uint16_t dutyCycle);
extern void setCH3PWMDutyCycle(uint16_t dutyCycle);
extern float ADCToCurrent(uint32_t adc);
extern float ADCToVoltage(uint32_t adc);

#endif