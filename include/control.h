#ifndef CONTROL_H
#define CONTROL_H
#include "common.h"
    
extern void setCH1PWMDutyCycle(uint16_t dutyCycle);
extern void setCH2PWMDutyCycle(uint16_t dutyCycle);
extern void setCH3PWMDutyCycle(uint16_t dutyCycle);
extern long getNanoTime();
extern void getADCValues();
extern double getAFeedBack();
extern double getBFeedBack();
extern double getCFeedBack();
extern double getACurrent();
extern double getBCurrent();
extern double getCCurrent();
extern double getPhaseControl();

#endif