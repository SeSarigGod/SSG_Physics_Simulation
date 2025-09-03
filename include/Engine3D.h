#ifndef PHYSICS_SIMULATION_ENGINE3D_H
#define PHYSICS_SIMULATION_ENGINE3D_H
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Object.h"

namespace SSG::PhysicsEngine
{
    class Engine3D
    {
    public:
        Engine3D(int screenWidth, int screenHeight);
        virtual ~Engine3D();
        virtual void addObject(Object* object);
        static void compile_shader(GLuint& shaderId, GLenum shaderType, const char* shaderFilePath);
        virtual void compileShaders(const std::vector<GLuint>& shaderTypes, const char* shaderFilePaths[]);
        virtual void compileShaders(const std::pair<const std::vector<GLuint>, const char* const*>& shaders);
        virtual void createCamera(const ldvec3& position, const ldvec3& front, const ldvec3& upVector);
        virtual void createCamera();
        void Debug(const std::vector<ldvec3>& points) const;
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
        int screenWidth = 2160;
        int screenHeight = 1440;

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
    };

    inline int width_ = 2160;
    inline int height_ = 1440;

    inline long double lastFrame = 0.0L;
    inline long double currentFrame = 0.0L;
    inline long double deltaTime = 0.0L;

    inline bool firstMouse = true;
    inline long double lastX = 0.0L;
    inline long double lastY = 0.0L;
    inline long double yaw = -90.0L;
    inline long double pitch = 0.0L;
    inline ldvec3 cameraF = ldvec3(0.0L, 0.0L, -1.0L);

    inline long double fov = 45.0L;
}

#endif //PHYSICS_SIMULATION_ENGINE3D_H
