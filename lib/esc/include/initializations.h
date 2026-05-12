#ifndef INITIALIZATIONS_H
#define INITIALIZATIONS_H
#pragma once

#include <common.h>

extern uint32_t adc_base;

void enableSystemClock();
void configureClocks();
void initITM();
void enableFPU();
void configureLEDs();
void enablePWM();
void configurePWM();
void configureADC();
void configureInterrupts();

#endif