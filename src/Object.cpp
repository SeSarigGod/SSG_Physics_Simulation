#include "Object.h"

#include <iostream>
#include <ostream>
#include <stdexcept>

#include "Camera.h"


Object::Object(const std::vector<double>& position, const std::vector<double>& velocity, const double mass, const ObjectType objectType, const double* objectParameters)
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

    this->acceleration = std::vector<double>(3, 0.0);
}

Object::Object(const std::vector<double>& position, const double mass, const ObjectType objectType, const double* objectParameters)
{
    this->objectType = objectType;
    this->objectParameters = objectParameters;

    if (objectType == ObjectType::SPHERE)
    {
        this->radius = *objectParameters;
    }

    this->position = position;
    this->mass = mass;

    this->velocity = std::vector<double>(3, 0.0);
    this->acceleration = std::vector<double>(3, 0.0);
}

Object::Object(const double mass, const ObjectType objectType, const double* objectParameters)
{
    this->objectType = objectType;
    this->objectParameters = objectParameters;

    if (objectType == ObjectType::SPHERE)
    {
        this->radius = *objectParameters;
    }

    this->mass = mass;
}

std::vector<GLfloat> Object::Construct() const
{
    if (this->objectType == ObjectType::SPHERE)
    {
        return ConstructSphere();
    }

    throw std::runtime_error("Object type not implemented");
}

std::vector<GLfloat> Object::ConstructSphere() const
{
    std::vector<GLfloat> vertices(36 * (res + 1) * (res + 0), 0.0);

    for (int i = 0; i <= res; ++i)
    {
        for (int j = 0; j < res; ++j)
        {
            const double theta = deltaTheta * static_cast<double>(i);
            const double phi = deltaPhi * static_cast<double>(j);
            const double theta_prime = deltaTheta * static_cast<double>(i + 1);
            const double phi_prime = deltaPhi * static_cast<double>(j + 1);

            const glm::vec3 v1 = Spherical2Cartesian(theta, phi);
            const glm::vec3 v2 = Spherical2Cartesian(theta, phi_prime);
            const glm::vec3 v3 = Spherical2Cartesian(theta_prime, phi);
            const glm::vec3 v4 = Spherical2Cartesian(theta_prime, phi_prime);

            const int index = 36 * (i * (res + 0) + j);

            vertices[index + 0] = v1.x + static_cast<float>(position[0]);
            vertices[index + 1] = v1.y + static_cast<float>(position[1]);
            vertices[index + 2] = v1.z + static_cast<float>(position[2]);
            vertices[index + 3] = 1.0;
            vertices[index + 4] = 0.0;
            vertices[index + 5] = 0.0;
            vertices[index + 6] = v2.x + static_cast<float>(position[0]);
            vertices[index + 7] = v2.y + static_cast<float>(position[1]);
            vertices[index + 8] = v2.z + static_cast<float>(position[2]);
            vertices[index + 9] = 0.0;
            vertices[index + 10] = 1.0;
            vertices[index + 11] = 0.0;
            vertices[index + 12] = v3.x + static_cast<float>(position[0]);
            vertices[index + 13] = v3.y + static_cast<float>(position[1]);
            vertices[index + 14] = v3.z + static_cast<float>(position[2]);
            vertices[index + 15] = 0.0;
            vertices[index + 16] = 0.0;
            vertices[index + 17] = 1.0;
            vertices[index + 18] = v2.x + static_cast<float>(position[0]);
            vertices[index + 19] = v2.y + static_cast<float>(position[1]);
            vertices[index + 20] = v2.z + static_cast<float>(position[2]);
            vertices[index + 21] = 1.0;
            vertices[index + 22] = 0.0;
            vertices[index + 23] = 0.0;
            vertices[index + 24] = v4.x + static_cast<float>(position[0]);
            vertices[index + 25] = v4.y + static_cast<float>(position[1]);
            vertices[index + 26] = v4.z + static_cast<float>(position[2]);
            vertices[index + 27] = 0.0;
            vertices[index + 28] = 1.0;
            vertices[index + 29] = 0.0;
            vertices[index + 30] = v3.x + static_cast<float>(position[0]);
            vertices[index + 31] = v3.y + static_cast<float>(position[1]);
            vertices[index + 32] = v3.z + static_cast<float>(position[2]);
            vertices[index + 33] = 0.0;
            vertices[index + 34] = 0.0;
            vertices[index + 35] = 1.0;
        }
    }

    return vertices;
}

std::vector<double> Object::DeltaPosition(const std::vector<double>& velocity, const std::vector<double>& acceleration, const double dt)
{
    std::vector<double> deltaPosition(velocity.size(), 0.0);
    for (int i = 0; i < velocity.size(); i++)
    {
        deltaPosition[i] = velocity[i] + acceleration[i] * dt;
    }

    return deltaPosition;
}

std::vector<double> Object::DeltaVelocity(const std::vector<double>& acceleration)
{
    std::vector<double> deltaVelocity(acceleration.size(), 0.0);
    for (int i = 0; i < acceleration.size(); i++)
    {
        deltaVelocity[i] = acceleration[i];
    }

    return deltaVelocity;
}

void Object::Draw()
{

}

std::vector<double> Object::getAcceleration() const
{
    return acceleration;
}

double Object::getMass() const
{
    return mass;
}

std::vector<double> Object::getPosition() const
{
    return position;
}

std::vector<double> Object::getVelocity() const
{
    return velocity;
}

void Object::HandleBounds(const std::pair<double, double>& x_bounds, const std::pair<double, double>& y_bounds, const std::pair<double, double>& z_bounds)
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
    if (this->position.size() > 2)
    {
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
}

void Object::HandleCollision(Object& other)
{
    std::cout << "Collision" << std::endl;
}

void Object::RK4Step(const double dt)
{
    const std::vector<double> k1_p = DeltaPosition(velocity, acceleration, dt);
    const std::vector<double> k2_p = DeltaPosition(k1_p, acceleration, dt / 2.0);
    const std::vector<double> k3_p = DeltaPosition(k2_p, acceleration, dt / 2.0);
    const std::vector<double> k4_p = DeltaPosition(k3_p, acceleration, dt);

    for (int i = 0; i < position.size(); i++)
    {
        position[i] += (k1_p[i] + 2 * k2_p[i] + 2 * k3_p[i] + k4_p[i]) / 6.0;
        velocity[i] += acceleration[i] * dt;
    }
}

void Object::setAcceleration(const std::vector<double>& _acceleration)
{
    this->acceleration = _acceleration;
}

void Object::setMass(const float _mass)
{
    this->mass = _mass;
}

void Object::setPosition(const std::vector<double>& _position)
{
    this->position = _position;
}

void Object::setVelocity(const std::vector<double>& _velocity)
{
    this->velocity = _velocity;
}

glm::dvec3 Object::Spherical2Cartesian(const double theta, const double phi) const
{
    return {std::cos(theta) * std::sin(phi) * this->radius,
               std::sin(theta) * std::sin(phi) * this->radius,
               std::cos(phi) * this->radius};
}

void Object::update(const std::vector<double>& _acceleration, const double dt)
{
    this->acceleration = _acceleration;
    this->RK4Step(dt);
}

void Object::update(const double dt)
{
    this->RK4Step(dt);
}
