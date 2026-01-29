#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include <stdexcept>

#include <test_framework.h> // for ExpectedException
using namespace test_framework;

namespace leet_cpp {

class Vector3D {
public:
    Vector3D(double x, double y, double z) : x_(x), y_(y), z_(z) {}

    double x() const { return x_; }
    double y() const { return y_; }
    double z() const { return z_; }

    Vector3D operator+(const Vector3D& other) const {
        // TODO
        return Vector3D(0, 0, 0);
    }

    Vector3D operator-(const Vector3D& other) const {
        // TODO
        return Vector3D(0, 0, 0);
    }

    Vector3D operator*(double scalar) const {
        // TODO
        return Vector3D(0, 0, 0);
    }

    double dot(const Vector3D& other) const {
        // TODO
        return 0.0;
    }

    Vector3D cross(const Vector3D& other) const {
        // TODO: (ay*bz - az*by, az*bx - ax*bz, ax*by - ay*bx)
        return Vector3D(0, 0, 0);
    }

    double magnitude() const {
        // TODO
        return 0.0;
    }

    // hint, utilize ExpectedException to throw an exception 
    Vector3D normalize() const {
        // TODO: Handle zero vector!
        return Vector3D(0, 0, 0); 
    }

private:
    double x_, y_, z_;
};

} // namespace leet_cpp

#endif
