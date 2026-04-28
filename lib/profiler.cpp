#include <common.h>
#include <profiler.h>
#include <constants.h>

float abs(float x) {
    return (x < 0.0f) ? -x : x;
}

float sign(float x) {
    return (x < 0.0f) ? -1.0f: 1.0f;
}

float minmag(float a, float b) {
    return (abs(a) > abs(b)) ? b : a;
}

class Profiler {
    public:
        Profiler(float kvGain, float kaGain, float j_max, float max_voltage)
            :kv(kvGain), ka(kaGain), j_max(max_j) {
                max_v = max_voltage / kv;
                max_a = max_voltage / ka;
            }

        float compute(float setpoint, float accel, float dt) {
            float error = setpoint - current;
            
            float v_down = accel * accel / (2.0f * max_j);

            float jerk = sign(error);

            if (abs(error) >= abs(v_down)) {
                // Deccellarating
                jerk *= -max_j;
            } else if (abs(accel) < max_a) {
                // Cruise
                jerk = 0;
            } else {
                // Accelerating
                jerk *= max_j;
            }

            return minmag(accel + jerk*dt, max_a);
        }
    
    private:
        float kv;
        float ka;
        float max_a;
        float max_j;
};
