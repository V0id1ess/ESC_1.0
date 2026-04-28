#include <constants.h>

class PIController {
    public:
        PIController(float kpGain, float kiGain)
            :kp(kpGain), ki(kiGain), integral(0.0) {}

        float compute(float setpoint, float measured_value, float dt) {
            float error = setpoint - measured_value;

            integral += error * dt;

            float output = kp * error + ki * integral;

            if (output > MAX_VOLTAGE) {
                output = MAX_VOLTAGE;
                integral -= error * dt;
            } else if (output < -MAX_VOLTAGE) {
                output = -MAX_VOLTAGE;
                integral -= error * dt;
            }

            return output;
        }
    
    private:
        float kp;
        float ki;
        float integral;
};