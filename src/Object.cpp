#include "Object.h"

#include <iostream>
#include <ostream>
#include <stdexcept>

#include "Camera.h"

using namespace SSG::PhysicsEngine;

Object::Object(const ldvec3& position, const ldvec3& velocity, const long double mass, const ObjectType objectType, const long double* objectParameters)
{
    this->objectType = objectType;
    this->objectParameters = objectParameters;

    if (objectType == ObjectType::SPHERE)
    {
        this->radius = *objectParameters;
    }

    this->position = position;
    this->velocity = velocity;
    this->mass = mass;

    this->acceleration = ldvec3(0.0L);
}

Object::Object(const ldvec3& position, const long double mass, const ObjectType objectType, const long double* objectParameters)
{
    this->objectType = objectType;
    this->objectParameters = objectParameters;

    if (objectType == ObjectType::SPHERE)
    {
        this->radius = *objectParameters;
    }

    this->position = position;
    this->mass = mass;

    this->velocity = ldvec3(0.0L);
    this->acceleration = ldvec3(0.0L);
}

Object::Object(const long double mass, const ObjectType objectType, const long double* objectParameters)
{
    this->objectType = objectType;
    this->objectParameters = objectParameters;

    if (objectType == ObjectType::SPHERE)
    {
        this->radius = *objectParameters;
    }

    this->mass = mass;
}

std::vector<ldvec3> Object::Construct() const
{
    if (this->objectType == ObjectType::SPHERE)
    {
        return ConstructSphere();
    }

    throw std::runtime_error("Object type not implemented");
}

std::vector<ldvec3> Object::ConstructSphere(const ldvec3& _position)
{
    constexpr long double deltaTheta = TAU / res;
    constexpr long double deltaPhi = TAU / res * 0.5L;
    std::vector<ldvec3> vertices(6 * (res + 1) * (res + 0));

    for (unsigned long long i = 0; i <= res; ++i)
    {
        for (unsigned long long j = 0; j < res; ++j)
        {
            const long double theta = deltaTheta * i;
            const long double phi = deltaPhi * j;
            const long double theta_prime = deltaTheta * (i + 1);
            const long double phi_prime = deltaPhi * (j + 1);

            const ldvec3 v1 = Spherical2Cartesian(theta, phi);
            const ldvec3 v2 = Spherical2Cartesian(theta, phi_prime);
            const ldvec3 v3 = Spherical2Cartesian(theta_prime, phi);
            const ldvec3 v4 = Spherical2Cartesian(theta_prime, phi_prime);

            const unsigned long long index = 6 * (i * (res + 0) + j);

            vertices[index + 0] = v1 + _position;
            vertices[index + 1] = v2 + _position;
            vertices[index + 2] = v3 + _position;
            vertices[index + 3] = v2 + _position;
            vertices[index + 4] = v4 + _position;
            vertices[index + 5] = v3 + _position;
        }
    }

    return vertices;
}

std::vector<ldvec3> Object::ConstructFibonacciSphere()
{
    std::vector<ldvec3> points;
    points.reserve(res);

    constexpr long double deltaAngle = TAU / PHI;
    for (unsigned long long i = 0; i < res; i++)
    {
        const long double t = i / res;
        const long double theta = std::acos(1.0L - 2.0L * t);
        const long double phi = deltaAngle * i;

        points.push_back(Spherical2Cartesian(theta, phi));
    }

    return points;
}

// Credit to Martin Roberts for the original optimized code and Fil for the further optimization of the pole gap.
// Personally modified to work in radians instead of degrees.
// https://extremelearning.com.au/how-to-evenly-distribute-points-on-a-sphere-more-effectively-than-the-canonical-fibonacci-lattice/
// https://observablehq.com/@fil/spherical-phyllotaxis
std::vector<ldvec3> Object::ConstructModifiedFibonacciSphere()
{
    std::vector<ldvec3> points;
    points.reserve(res);

    constexpr long double thetaStep = TAU / PHI;
    constexpr long double poleGap = 0.7012L;
    constexpr long double phiStep = 2.0L / (res - 1.0L + 2.0L * poleGap);
    constexpr long double phiStart = -1.0L + phiStep * poleGap;
    points.push_back(Spherical2Cartesian(0.0L, -PI_2));
    if (res <= 1) return {};
    for (unsigned long long i = 0; i < res - 1; i++)
    {
        const long double theta = thetaStep * i - std::round(i / PHI);
        const long double phi = PI * std::asin(phiStart + i * phiStep);

        points.push_back(Spherical2Cartesian(theta, phi));
    }
    points.push_back(Spherical2Cartesian(0.0L, PI_2));

    return points;
}

ldvec3 Object::DeltaPosition(const ldvec3& velocity, const ldvec3& acceleration, const long double dt)
{
    const ldvec3 deltaPosition = velocity + acceleration * dt;

    return deltaPosition;
}

ldvec3 Object::DeltaVelocity(const ldvec3& acceleration)
{
    const ldvec3 deltaVelocity = acceleration;

    return deltaVelocity;
}

void Object::Draw()
{

}

ldvec3 Object::getAcceleration() const
{
    return acceleration;
}

long double Object::getMass() const
{
    return mass;
}

ldvec3 Object::getPosition() const
{
    return position;
}

ldvec3 Object::getVelocity() const
{
    return velocity;
}

void Object::HandleBounds(const std::pair<long double, long double>& x_bounds, const std::pair<long double, long double>& y_bounds, const std::pair<long double, long double>& z_bounds)
{
    if (position[0] < x_bounds.first)
    {
        position[0] = x_bounds.first;
        velocity[0] *= -1;
    }
    else if (position[0] > x_bounds.second)
    {
        position[0] = x_bounds.second;
        velocity[0] *= -1;
    }
    if (position[1] < y_bounds.first)
    {
        position[1] = y_bounds.first;
        velocity[1] *= -1;
    }
    else if (position[1] > y_bounds.second)
    {
        position[1] = y_bounds.second;
        velocity[1] *= -1;
    }
    if (position[2] < z_bounds.first)
    {
        position[2] = z_bounds.first;
        velocity[2] *= -1;
    }
    else if (position[2] > z_bounds.second)
    {
        position[2] = z_bounds.second;
        velocity[2] *= -1;
    }
}

void Object::HandleCollision(Object& other)
{
    std::cout << "Collision" << std::endl;
}

ldvec3 Object::InverseStereoProjection(const ldvec2& point)
{
    const long double denom = 1.0L + point.x * point.x + point.y * point.y;

    return {2.0L * point.x / denom,
               2.0L * point.y / denom,
               1.0L - 2.0L / denom};
}

void Object::RK4Step(const long double dt)
{
    const ldvec3 k1_p = DeltaPosition(velocity, acceleration, dt);
    const ldvec3 k2_p = DeltaPosition(k1_p, acceleration, dt / 2.0);
    const ldvec3 k3_p = DeltaPosition(k2_p, acceleration, dt / 2.0);
    const ldvec3 k4_p = DeltaPosition(k3_p, acceleration, dt);

    this->position += (k1_p + 2.0L * k2_p + 2.0L * k3_p + k4_p) / 6.0L;
    this->velocity += this->acceleration * dt;
}

void Object::setAcceleration(const ldvec3& _acceleration)
{
    this->acceleration = _acceleration;
}

void Object::setMass(const long double _mass)
{
    this->mass = _mass;
}

void Object::setPosition(const ldvec3& _position)
{
    this->position = _position;
}

void Object::setVelocity(const ldvec3& _velocity)
{
    this->velocity = _velocity;
}

ldvec3 Object::Spherical2Cartesian(const long double theta, const long double phi)
{
    return {std::cos(theta) * std::sin(phi),
               std::sin(theta) * std::sin(phi),
               std::cos(phi)};
}

ldvec2 Object::StereoProjection(const ldvec3& point)
{
    return {point.x / (1.0L - point.z),
               point.y / (1.0L - point.z)};
}

void Object::update(const ldvec3& _acceleration, const long double dt)
{
    this->acceleration = _acceleration;
    this->RK4Step(dt);
}

void Object::update(const long double dt)
{
    this->RK4Step(dt);
}
