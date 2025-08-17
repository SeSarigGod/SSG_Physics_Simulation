#ifndef PHYSICS_SIMULATION_OBJECTS_H
#define PHYSICS_SIMULATION_OBJECTS_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <numbers>

#include "Camera.h"


inline constexpr double two_pi = std::numbers::pi * 2.f;
inline constexpr int res = 100;
inline constexpr float deltaTheta = two_pi / static_cast<double>(res);
inline constexpr float deltaPhi = (two_pi / static_cast<double>(res)) * 0.5;

enum ObjectType
{
    SPHERE,
    CUBE,
    CYLINDER,
    RECTANGLE,
    PLANE,
    MESH,
};

class Object
{
public:
    Object(const std::vector<double>& position, const std::vector<double>& velocity, double mass, ObjectType objectType, const double* objectParameters);
    Object(const std::vector<double>& position, double mass, ObjectType objectType, const double* objectParameters);
    Object(double mass, ObjectType objectType, const double* objectParameters);
    Object() = default;
    virtual ~Object() = default;
    [[nodiscard]] virtual std::vector<GLfloat> Construct() const;
    [[nodiscard]] virtual std::vector<GLfloat> ConstructSphere() const;
    static std::vector<double> DeltaPosition(const std::vector<double>& velocity, const std::vector<double>& acceleration,
                                             double dt);
    static std::vector<double> DeltaVelocity(const std::vector<double>& acceleration);
    virtual void Draw();
    [[nodiscard]] virtual std::vector<double> getAcceleration() const;
    [[nodiscard]] virtual double getMass() const;
    [[nodiscard]] virtual std::vector<double> getPosition() const;
    [[nodiscard]] virtual std::vector<double> getVelocity() const;
    virtual void HandleBounds(const std::pair<double, double>& x_bounds,
                              const std::pair<double, double>& y_bounds,
                              const std::pair<double, double>& z_bounds);
    virtual void HandleCollision(Object& other);
    virtual void RK4Step(double dt);
    virtual void setAcceleration(const std::vector<double>& _acceleration);
    virtual void setMass(float _mass);
    virtual void setPosition(const std::vector<double>& _position);
    virtual void setVelocity(const std::vector<double>& _velocity);
    [[nodiscard]] glm::dvec3 Spherical2Cartesian(double theta, double phi) const;
    virtual void update(const std::vector<double>& _acceleration, double dt);
    virtual void update(double dt);

protected:
    ObjectType objectType{};

    std::vector<double> position = std::vector<double>(3, 0.0);
    std::vector<double> velocity = std::vector<double>(3, 0.0);
    std::vector<double> acceleration = std::vector<double>(3, 0.0);

    double mass = 1.0;
    const double* objectParameters{};

    double radius = 0;
    double sideLength = 0;
    double height = 0;
    double width = 0;
    double length = 0;
};


#endif //PHYSICS_SIMULATION_OBJECTS_H