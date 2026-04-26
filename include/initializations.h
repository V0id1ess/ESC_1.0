#ifndef INITIALIZATIONS_H
#define INITIALIZATIONS_H

#include <common.h>

extern uint32_t adc_base;

extern void enableSystemClock();
extern void configureClocks();
extern void enableFPU();
extern void configureLEDs();
extern void enablePWM();
extern void configurePWM();
extern void configureADC();
extern void configureInterrupts();

#endif