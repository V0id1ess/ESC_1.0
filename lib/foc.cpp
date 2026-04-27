#include <trig.h>
#include <foc.h>

const float SQRT3 = 1.73205080757;
const float RECIP_SQRT3 = 1.0/SQRT3;

// stationary frame to rotating frame
void park(Vector2D stat, float theta, Vector2D &res) {
    res.x = stat.x * cos(theta) + stat.y * sin(theta); // I_d
    res.y = -stat.x * sin(theta) + stat.y * cos(theta); // I_q
}

// natural frame to stationary frame
void clarke(Vector3D natural, Vector2D &res) {
    res.x = natural.x; // I_alpha
    res.y = RECIP_SQRT3*(natural.x + 2.0f*natural.z); // I_beta
}

// rotating frame to stationary frame
void park_inverse(Vector2D rot, float theta, Vector2D &res) {
    res.x = rot.x * cos(theta) - rot.y * sin(theta); // I_alpha
    res.y = rot.x * sin(theta) + rot.y * cos(theta); // I_beta
}

// stationary frame to natural frame
void clarke_inverse(Vector2D stat, Vector3D &res) {
    res.x = stat.x; // I_a
    res.y = -0.5f * stat.x + SQRT3 * 0.5f * stat.y; // I_b
    res.z = -0.5f * stat.x - SQRT3 * 0.5f * stat.y; // I_c
}