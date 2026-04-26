#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <common.h>

// Mathematical
#define PI 3.14159265358979323846

// Clock
#define SYSCLK 64000000 // 64MHz
#define APB2CLK 64000000 // 64MHz
#define APB1CLK 32000000 // 32MHz

// Interface
#define PWM 25000 // 25kHz
#define PWM_ARR (static_cast<uint32_t> (SYSCLK / PWM - 1))
#define VREF 3.3 // 3.3V

// Tuning Parameters
#define Id_p 0.0 // Id PI Controller Proportional Gain
#define Id_i 0.0 // Id PI Controller Integral Gain
#define Iq_p 0.0 // Iq PI Controller Proportional Gain
#define Iq_i 0.0 // Iq PI Controller Integral Gain

// Mechanical
#define POLES 12
#define MAX_VOLTAGE 25.2
#define MAX_CURRENT 3

#endif