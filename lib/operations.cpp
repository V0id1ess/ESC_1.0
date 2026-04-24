#include <trig.h>

const float SQRT3 = 1.73205080757;
const float RECIP_SQRT3 = 1.0/SQRT3;

struct Vector2D {
    float x;
    float y;
};

struct Vector3D {
    float x;
    float y;
    float z;
};


void park(float I_alpha, float I_beta, float theta, Vector2D &res) {
    res.x = I_alpha * cos(theta) + I_beta * sin(theta); // I_d
    res.y = -I_alpha * sin(theta) + I_beta * cos(theta); // I_q
}

void clarke(float I_a, float I_c, Vector2D &res) {
    res.x = I_a; // I_alpha
    res.y = RECIP_SQRT3*(I_a + 2.0f*I_c); // I_beta
}


void park_inverse(float I_d, float I_q, float theta, Vector2D &res) {
    res.x = I_d * cos(theta) - I_q * sin(theta); // I_alpha
    res.y = I_d * sin(theta) + I_q * cos(theta); // I_beta
}

void clarke_inverse(float I_alpha, float I_beta, Vector3D &res) {
    res.x = I_alpha; // I_a
    res.y = -0.5f * I_alpha + SQRT3 / 2.0f * I_beta; // I_b
    res.z = -0.5f * I_alpha - SQRT3 / 2.0f * I_beta; // I_c
}