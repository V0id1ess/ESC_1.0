#ifndef OPERATIONS_H
#define OPERATIONS_H
#pragma once
#include <trig.h>

struct Vector2D {
    float x;
    float y;
};

struct Vector3D {
    float x;
    float y;
    float z;
};

static const float SQRT3 = 1.73205080757;
static const float RECIP_SQRT3 = 1.0/SQRT3;

static float sin, cos;

// stationary frame to rotating frame
inline void park(Vector2D stat, float theta, Vector2D &res) {
    trigf(theta, sin, cos);
    res.x = stat.x * cos + stat.y * sin; // I_d
    res.y = -stat.x * sin + stat.y * cos; // I_q
}

// natural frame to stationary frame
inline void clarke(Vector3D natural, Vector2D &res) {
    res.x = natural.x; // I_alpha
    res.y = RECIP_SQRT3*(natural.x + 2.0f*natural.y); // I_beta
}

// rotating frame to stationary frame
inline void park_inverse(Vector2D rot, float theta, Vector2D &res) {
    res.x = rot.x * cos - rot.y * sin; // I_alpha
    res.y = rot.x * sin + rot.y * cos; // I_beta
}

// stationary frame to natural frame
inline void clarke_inverse(Vector2D stat, Vector3D &res) {
    res.x = stat.x; // I_a
    res.y = -0.5f * stat.x + SQRT3 * 0.5f * stat.y; // I_b
    res.z = -0.5f * stat.x - SQRT3 * 0.5f * stat.y; // I_c
}

#endif