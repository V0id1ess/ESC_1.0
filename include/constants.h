#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <common.h>

// Mathematical
#define PI 3.14159265358979323846

// Clock
#define SYSCLK 64000000 // 64MHz
#define APB2CLK 64000000 // 64MHz
#define APB1CLK 32000000 // 32MHz
#define PWM 25000 // 25kHz
#define PWM_ARR (uint32_t) (SYSCLK / PWM - 1)

// Mechanical
#define POLES 12

#endif