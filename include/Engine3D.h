#ifndef PHYSICS_SIMULATION_ENGINE3D_H
#define PHYSICS_SIMULATION_ENGINE3D_H
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Object.h"


class Engine3D
{
public:
    Engine3D(int screenWidth, int screenHeight);
    virtual ~Engine3D();
    virtual void addObject(Object* object);
    static void compile_shader(GLuint& shaderId, GLenum shaderType, const char* shaderFilePath);
    virtual void compileShaders(const std::vector<GLuint>& shaderTypes, const char* shaderFilePaths[]);
    virtual void compileShaders(const std::pair<const std::vector<GLuint>, const char* const*>& shaders);
    virtual void createCamera(const glm::dvec3& position, const glm::dvec3& front, const glm::dvec3& upVector);
    virtual void createCamera();
    virtual void Draw();
    static void framebuffer_size_callback(GLFWwindow* /*win*/, int width, int height);
    virtual Camera* getCamera();
    static char* get_shader_content(const char* fileName);
    [[nodiscard]] virtual GLFWwindow* getWindow() const;
    virtual void link_shader();
    void mainLoop();
    static void mouse_callback(GLFWwindow* /*win*/, double xpos, double ypos);
    static void scroll_callback(GLFWwindow*, double xoffset, double yoffset);
    static void processInput(GLFWwindow* window);
    virtual void processInput() const;
    virtual void startGLFW();
    virtual void setScreenSize(int width, int height);

protected:
    int screenWidth = 1280;
    int screenHeight = 720;

    GLFWwindow* window = nullptr;
    Camera* camera = nullptr;

    GLuint VAO{}, VBO{};
    std::vector<GLuint> shaderIDs{};
    GLuint programID{};

    std::vector<Object*> objects{};
    // double roll = 0.0f;

    GLint viewMatrixID{};
    GLint projectionMatrixID{};
    GLint modelMatrixID{};
    // GLint normalMatrixID{};
    // GLint viewProjectionMatrixID{};
    // GLint cameraPositionID{};
    // GLint lightPositionID{};
    // GLint lightColorID{};
    // GLint lightAmbientID{};
};

inline int width_ = 1280;
inline int height_ = 720;

inline double lastFrame = 0.0;
inline double currentFrame = 0.0;
inline double deltaTime = 0.0;

inline bool firstMouse = true;
inline double lastX = 0;
inline double lastY = 0;
inline double yaw = -90.0f;
inline double pitch = 0.0f;
inline glm::dvec3 cameraF = glm::vec3(0.0, 0.0, -1.0);

inline double fov = 45.0;

#endif //PHYSICS_SIMULATION_ENGINE3D_H
