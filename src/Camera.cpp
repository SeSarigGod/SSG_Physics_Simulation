#include "Camera.h"

using namespace SSG::PhysicsEngine;

Camera::Camera(const ldvec3& position, const ldvec3& target, const ldvec3& upVector)
{
    this->cameraPos = position;
    this->cameraTarget = target;
    this->upVector = upVector;
    this->cameraFront = ldvec3(0.0, 0.0, -1.0);
    this->cameraDirection = glm::normalize(this->cameraPos - this->cameraTarget);
    this->cameraRight = glm::normalize(glm::cross(this->upVector, this->cameraDirection));
    this->cameraUp = glm::normalize(glm::cross(this->cameraDirection, this->cameraRight));
    this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraFront, this->upVector);
}

ldvec3 Camera::getDirection() const
{
    return cameraDirection;
}

ldvec3 Camera::getPosition() const
{
    return cameraPos;
}

ldvec3 Camera::getTarget() const
{
    return cameraTarget;
}

ldvec3 Camera::getUpVector() const
{
    return upVector;
}

ldmat4 Camera::getViewMatrix() const
{
    return viewMatrix;
}

void Camera::processInput(GLFWwindow *window, const long double deltaTime)
{
    const long double cameraSpeed = 2.0 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * upVector;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * upVector;
    }
    this->Update();
}

void Camera::setDirection(const ldvec3& direction)
{
    this->cameraDirection = glm::normalize(direction);
    this->Update();
}

void Camera::setFront(const ldvec3& front)
{
    this->cameraFront = glm::normalize(front);
    this->Update();
}

void Camera::setPosition(const ldvec3& position)
{
    this->cameraPos = position;
    this->Update();
}

void Camera::setTarget(const ldvec3& target)
{
    this->cameraTarget = target;
    this->Update();
}

void Camera::setUpVector(const ldvec3& up)
{
    this->upVector = up;
    this->Update();
}

void Camera::Update()
{
    this->cameraDirection = glm::normalize(this->cameraPos - this->cameraTarget);
    this->cameraRight = glm::normalize(glm::cross(this->upVector, this->cameraDirection));
    this->cameraUp = glm::normalize(glm::cross(this->cameraDirection, this->cameraRight));
    this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->upVector);
}