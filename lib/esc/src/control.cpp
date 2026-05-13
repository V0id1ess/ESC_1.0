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
static float angle; // Electrical angle

static float Iq_setpoint;

static PIDController IdController(Id_p, Id_i, 0.0, DT);
static PIDController IqController(Iq_p, Iq_i, 0.0, DT);
static PIDController VelocityFBController(Vel_p, Vel_i, Vel_d, DT);
static Feedforward VelocityFFController(PROFILER_KV, PROFILER_KA);
static Profiler MotionProfiler(PROFILER_KV, PROFILER_KA, PROFILER_JERK_MAX);

static float prevTime;

volatile uint64_t frameCount = 0;

void FOC_update() {
    // Stationary Frame Transform
    clarke(I, Istat);

    // Rotating Frame Transform
    park(Istat, angle, Irot);

    
    if ((frameCount & (127)) == 0) { // Every 32 frames, update the SMO and control loops
        SMO_update();

        // Position Generator (SMO)
        if (motorState == ALIGN) {
            angle = 0.0f; // Force rotor to a known position
            throttle = 0.5f; // Apply moderate throttle to encourage alignment
            align_time += DT;
            if (align_time >= 0.500f) { // After 500 ms, transition to LOW_BEMF
                motorState = LOW_BEMF;
            }
        } else if (motorState == LOW_BEMF) {
            incrementVirtualAngle(1.0f); // Slowly ramp up virtual angle to encourage movement
            angle = getAngle();
        } else if (motorState == RUNNING) {
            angle = getAngle();
        }

        // Motion Profiling and Perpendicular Current Control
        MotionProfiler.compute(throttle, profile, targetProfile, DT);
        float I_fb = VelocityFBController.compute(targetProfile.velocity, profile.velocity);
        float I_ff = VelocityFFController.compute(targetProfile);
        Iq_setpoint = I_fb + I_ff;

        // Iq -> V_sq, Id -> V_sd
        Vrot.x = IdController.compute(0.0f, Irot.x); // V_sd
        Vrot.y = IqController.compute(Iq_setpoint, Irot.y); // V_sq
    }

    // Back to Stationary Frame
    park_inverse(Vrot, angle, Vstat);

    // Back to Natural Frame and PWM Outputs
    // At some point, switch to Space Vector PWM for better performance
    clarke_inverse(Vstat, signals);

    setDutyCycles(signals);

    frameCount++;
}
