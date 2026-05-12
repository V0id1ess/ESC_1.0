#include <esc.h>

// Global Variables
ControlState motorState = ALIGN;

Vector3D I;
Vector3D V;
Vector3D I_offset;

Vector2D Istat, Vstat; // Stationary Reference Frame (alpha-beta)
Vector2D Irot, Vrot; // Rotating Reference Frame (d-q)
Vector3D signals; // SVPWM Duty Cycle Signals for 3 Phases

// Motion Variables
float jerk; // Current jerk (testing use only)
float throttle; // 0-100% Throttle Command
Profile profile; // Motion Profile for Velocity and Acceleration
Profile targetProfile; // Target Motion Profile for Velocity and Acceleration

float align_time = 0.0f; // Time spent in ALIGN state
float angle; // Electrical angle

float Iq_setpoint;

PIDController IdController(Id_p, Id_i, 0.0);
PIDController IqController(Iq_p, Iq_i, 0.0);
PIDController VelocityFBController(Vel_p, Vel_i, Vel_d);
Feedforward VelocityFFController(PROFILER_KV, PROFILER_KA);
Profiler MotionProfiler(PROFILER_KV, PROFILER_KA, PROFILER_JERK_MAX);

float prevTime;

volatile uint64_t frameCount = 0;

void FOC_update() {
    // Stationary Frame Transform
    clarke(I, Istat);

    if (frameCount && (0b1111) == (0b1000)) {
        SMO_update();
    }

    // Position Generator (SMO)
    // switch(motorState) {
    //     case ALIGN:
    //         angle = 0.0f; // Force rotor to a known position
    //         throttle = 0.5f; // Apply moderate throttle to encourage alignment
    //         align_time += DT;
    //         if (align_time >= 0.500f) { // After 500 ms, transition to STARTING
    //             motorState = LOW_BEMF;
    //         }

    //         break;
        
    //     case LOW_BEMF:
    //         incrementVirtualAngle(1.0f); // Slowly ramp up virtual angle to encourage movement
    //         angle = getAngle();
    //         break;
        
    //     case RUNNING:
    //         angle = getAngle();
    //         break;
    // }

    // Rotating Frame Transform
    park(Istat, angle, Irot);

    
    if (frameCount && (0b1111) == (0b1000)) {
        // Motion Profiling and Perpendicular Current Control
        MotionProfiler.compute(throttle, profile, targetProfile, DT);
        float I_fb = VelocityFBController.compute(targetProfile.velocity, profile.velocity, DT);
        float I_ff = VelocityFFController.compute(targetProfile);
        Iq_setpoint = I_fb + I_ff;

        // Iq -> V_sq, Id -> V_sd
        Vrot.x = IdController.compute(0.0f, Irot.x, DT); // V_sd
        Vrot.y = IqController.compute(Iq_setpoint, Irot.y, DT); // V_sq
    }

    // Back to Stationary Frame
    park_inverse(Vrot, angle, Vstat);

    // Back to Natural Frame and PWM Outputs
    // At some point, switch to Space Vector PWM for better performance
    clarke_inverse(Vstat, signals);

    setDutyCycles(signals);
}
