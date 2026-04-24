#ifndef PI_H
#define PI_H

class PI {
    public:
        PI(double kpGain, double kiGain);
        double compute(double setpoint, double measured_value, double dt);
};

#endif