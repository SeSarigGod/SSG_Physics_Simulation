#ifndef PHYSICS_SIMULATION_CAMERA_H
#define PHYSICS_SIMULATION_CAMERA_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "LongDoubleGLMExtension.h"

namespace SSG::PhysicsEngine
{
    class Camera
    {
    public:
        Camera(const ldvec3& position, const ldvec3& target, const ldvec3& upVector);
        Camera() = default;
        ~Camera() = default;

        [[nodiscard]] ldvec3 getDirection() const;
        [[nodiscard]] ldvec3 getPosition() const;
        [[nodiscard]] ldvec3 getTarget() const;
        [[nodiscard]] ldvec3 getUpVector() const;
        [[nodiscard]] ldmat4 getViewMatrix() const;
        void processInput(GLFWwindow *window, long double deltaTime);
        void setDirection(const ldvec3& direction);
        void setFront(const ldvec3& front);
        void setPosition(const ldvec3& position);
        void setTarget(const ldvec3& target);
        void setUpVector(const ldvec3& up);
        void Update();

    protected:
        ldvec3 cameraPos = ldvec3(0.0L, 0.0L, 5.0L);
        ldvec3 cameraTarget = ldvec3(0.0L, 0.0L, 0.0L);
        ldvec3 upVector = ldvec3(0.0L, 1.0L, 0.0L);
        ldvec3 cameraFront = ldvec3(0.0L, 0.0L, -1.0L);

        ldvec3 cameraDirection = glm::normalize(this->cameraPos - this->cameraTarget);
        ldvec3 cameraRight = glm::normalize(glm::cross(this->upVector, this->cameraDirection));
        ldvec3 cameraUp = glm::normalize(glm::cross(this->cameraDirection, this->cameraRight));
        ldmat4 viewMatrix = glm::lookAt(this->cameraPos, this->cameraTarget, this->upVector);
    };
}

#endif //PHYSICS_SIMULATION_CAMERA_H