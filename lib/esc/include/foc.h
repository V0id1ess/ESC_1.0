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

static const float ONE_OVER_SQRT3 = 0.577350269f;
static const float SQRT3_OVER_2   = 0.866025403f;

static float _sinVal, _cosVal;

// stationary frame to rotating frame
inline void park(Vector2D stat, float theta, Vector2D &res) {
    trigf(theta, _sinVal, _cosVal);
    res.x = stat.x * _cosVal + stat.y * _sinVal; // I_d
    res.y = -stat.x * _sinVal + stat.y * _cosVal; // I_q
}

// natural frame to stationary frame
inline void clarke(Vector3D natural, Vector2D &res) {
    res.x = natural.x;  // I_alpha
    res.y = (natural.x + 2.0f * natural.y) * ONE_OVER_SQRT3; // I_beta
}

// rotating frame to stationary frame
inline void park_inverse(Vector2D rot, float theta, Vector2D &res) {
    res.x = rot.x * _cosVal - rot.y * _sinVal; // I_alpha
    res.y = rot.x * _sinVal + rot.y * _cosVal; // I_beta
}

// stationary frame to natural frame
inline void clarke_inverse(const Vector2D& stat, Vector3D& res) {
    float half_alpha = 0.5f * stat.x;
    float beta_calc  = stat.y * SQRT3_OVER_2;
    
    res.x = stat.x;
    res.y = -half_alpha + beta_calc;
    res.z = -half_alpha - beta_calc;
}

#endif