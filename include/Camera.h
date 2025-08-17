#ifndef PHYSICS_SIMULATION_CAMERA_H
#define PHYSICS_SIMULATION_CAMERA_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
    Camera(const glm::dvec3& position, const glm::dvec3& target, const glm::dvec3& upVector);
    Camera() = default;
    ~Camera() = default;

    [[nodiscard]] glm::dvec3 getDirection() const;
    [[nodiscard]] glm::dvec3 getPosition() const;
    [[nodiscard]] glm::dvec3 getTarget() const;
    [[nodiscard]] glm::dvec3 getUpVector() const;
    [[nodiscard]] glm::dmat4 getViewMatrix() const;
    void processInput(GLFWwindow *window, double deltaTime);
    void setDirection(const glm::dvec3& direction);
    void setFront(const glm::dvec3& front);
    void setPosition(const glm::dvec3& position);
    void setTarget(const glm::dvec3& target);
    void setUpVector(const glm::dvec3& up);
    void Update();

protected:
    glm::dvec3 cameraPos = glm::dvec3(0.0, 0.0, 5.0);
    glm::dvec3 cameraTarget = glm::dvec3(0.0, 0.0, 0.0);
    glm::dvec3 upVector = glm::dvec3(0.0, 1.0, 0.0);
    glm::dvec3 cameraFront = glm::dvec3(0.0, 0.0, -1.0);

    glm::dvec3 cameraDirection = glm::normalize(this->cameraPos - this->cameraTarget);
    glm::dvec3 cameraRight = glm::normalize(glm::cross(this->upVector, this->cameraDirection));
    glm::dvec3 cameraUp = glm::normalize(glm::cross(this->cameraDirection, this->cameraRight));
    glm::dmat4 viewMatrix = glm::lookAt(this->cameraPos, this->cameraTarget, this->upVector);
};


#endif //PHYSICS_SIMULATION_CAMERA_H