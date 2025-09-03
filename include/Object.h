#ifndef PHYSICS_SIMULATION_OBJECTS_H
#define PHYSICS_SIMULATION_OBJECTS_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <numbers>

#include "Camera.h"


inline constexpr long double PI = 3.14159265358979323846264338327L;
inline constexpr long double TAU = PI * 2.0L;
inline constexpr long double PI_2 = PI / 2.0L;
inline constexpr long double DEGREES = 180.0L / PI;
inline constexpr long double PHI = 1.61803398874989484820458683436L;
inline constexpr unsigned long long res = 10000;

namespace SSG::PhysicsEngine
{
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
        Object(const ldvec3& position, const ldvec3& velocity, long double mass, ObjectType objectType, const long double* objectParameters);
        Object(const ldvec3& position, long double mass, ObjectType objectType, const long double* objectParameters);
        Object(long double mass, ObjectType objectType, const long double* objectParameters);
        Object() = default;
        virtual ~Object() = default;
        [[nodiscard]] virtual std::vector<ldvec3> Construct() const;
        [[nodiscard]] static std::vector<ldvec3> ConstructSphere(const ldvec3& _position = {0.0L, 0.0L, 0.0L});
        [[nodiscard]] static std::vector<ldvec3> ConstructFibonacciSphere();
        [[nodiscard]] static std::vector<ldvec3> ConstructModifiedFibonacciSphere();
        static ldvec3 DeltaPosition(const ldvec3& velocity, const ldvec3& acceleration,
                                              long double dt);
        static ldvec3 DeltaVelocity(const ldvec3& acceleration);
        virtual void Draw();
        [[nodiscard]] virtual ldvec3 getAcceleration() const;
        [[nodiscard]] virtual long double getMass() const;
        [[nodiscard]] virtual ldvec3 getPosition() const;
        [[nodiscard]] virtual ldvec3 getVelocity() const;
        virtual void HandleBounds(const std::pair<long double, long double>& x_bounds,
                                  const std::pair<long double, long double>& y_bounds,
                                  const std::pair<long double, long double>& z_bounds);
        virtual void HandleCollision(Object& other);
        static ldvec3 InverseStereoProjection(const ldvec2& point);
        virtual void RK4Step(long double dt);
        virtual void setAcceleration(const ldvec3& _acceleration);
        virtual void setMass(long double _mass);
        virtual void setPosition(const ldvec3& _position);
        virtual void setVelocity(const ldvec3& _velocity);
        static ldvec3 Spherical2Cartesian(long double theta, long double phi);
        static ldvec2 StereoProjection(const ldvec3& point);
        virtual void update(const ldvec3& _acceleration, long double dt);
        virtual void update(long double dt);

    protected:
        ObjectType objectType{};

        ldvec3 position = ldvec3(0.0);
        ldvec3 velocity = ldvec3(0.0);
        ldvec3 acceleration = ldvec3(0.0);

        long double mass = 1.0;
        const long double* objectParameters{};

        long double radius = 0;
        long double sideLength = 0;
        long double height = 0;
        long double width = 0;
        long double length = 0;
    };
}

#endif //PHYSICS_SIMULATION_OBJECTS_H