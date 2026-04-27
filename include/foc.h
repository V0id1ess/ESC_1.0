#ifndef OPERATIONS_H
#define OPERATIONS_H

struct Vector2D {
    float x;
    float y;
};

struct Vector3D {
    float x;
    float y;
    float z;
};

extern void park(Vector2D stat, float theta, Vector2D &res);
extern void clarke(Vector3D natural, Vector2D &res);

extern void park_inverse(Vector2D rot, float theta, Vector2D &res);
extern void clarke_inverse(Vector2D stat, Vector3D &res);

#endif