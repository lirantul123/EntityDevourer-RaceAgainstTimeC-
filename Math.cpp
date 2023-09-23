#include <iostream>
#include "holder.h"

struct Vector2 {
    float x;
    float y;

    float distance(const Vector2& other) const {
        float dx = x - other.x;
        float dy = y - other.y;

        return std::sqrt(dx * dx + dy * dy);
    }
};

struct Vector3 {
    float x;
    float y;
    float z;

    double distance(const Vector3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;


        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
};
