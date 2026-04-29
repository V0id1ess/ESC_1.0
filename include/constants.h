#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <common.h>

// Mathematical
#define PI 3.14159265358979323846

// Clock
#define SYSCLK 64000000UL // 64MHz
#define APB2CLK 64000000UL // 64MHz
#define APB1CLK 32000000UL // 32MHz

// Interface
#define PWM 25000UL // 25kHz
#define PWM_ARR (static_cast<uint32_t>(SYSCLK) / static_cast<uint32_t>(PWM) - 1UL)
#define VREF 3.3f // 3.3V
#define DT 1.0f/PWM // Time step for control loop (seconds)

// Tuning Parameters
#define Id_p 0.0f // Id PI Controller Proportional Gain
#define Id_i 0.0f // Id PI Controller Integral Gain
#define Iq_p 0.0f // Iq PI Controller Proportional Gain
#define Iq_i 0.0f // Iq PI Controller Integral Gain
#define Vel_p 0.0f // Velocity PID Controller Proportional Gain
#define Vel_i 0.0f // Velocity PID Controller Integral Gain
#define Vel_d 0.0f // Velocity PID Controller Derivative Gain
#define MIN_BEMF 0.5f // Minimum Back-EMF Voltage for Reliable Angle Estimation (Volts)
#define GAIN 26.0f // Gain for SMO Back-EMF Update (Higher = Faster Response, but More Noise)
#define PHI 0.5f // Smoothing factor for angle estimation (0.0 - 1.0, higher = smoother but more lag)
#define LOW_PASS_ALPHA 0.1f // Alpha for low-pass filtering of angle estimation (0.0 - 1.0, higher = smoother but more lag)
#define PROFILER_KV 0.0f // Motion Profiler Velocity Gain
#define PROFILER_KA 0.0f // Motion Profiler Acceleration Gain
#define PROFILER_JERK_MAX 0.0f // Motion Profiler Maximum Jerk (Rate of Change of Acceleration)

// Mechanical
#define POLES 12.0f
#define MAX_VOLTAGE 25.2f // 25.2 Volts
#define MAX_CURRENT 3.0f // 3 Amps
#define INTERNAL_RESISTANCE 0.11f // 0.11 Ohms
#define INTERNAL_INDUCTANCE 0.000016f // 16uH
#define CONST_F 1.0f - (INTERNAL_RESISTANCE * DT) / INTERNAL_INDUCTANCE // Pre-calculated constant for current estimation
#define CONST_K DT / INTERNAL_INDUCTANCE // Pre-calculated constant for current estimation
#define MOTOR_KV 2200.0f // RPM per Volt
#define f_elec_MAX (MOTOR_KV * MAX_VOLTAGE * POLES) / 120.0f // Maximum Electrical Frequency (Hz)

#endif