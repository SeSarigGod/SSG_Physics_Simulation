#include "Engine3D.h"

#include <cstring>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace SSG::PhysicsEngine;

Engine3D::Engine3D(const int screenWidth, const int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    width_ = screenWidth;
    height_ = screenHeight;

    lastX = screenWidth / 2.0;
    lastY = screenHeight / 2.0;

    Engine3D::createCamera();

    Engine3D::startGLFW();
    glewInit();
    if (!this->window) return;

    const char* shaderPaths[] = {"../shaders/basic.vert", "../shaders/basic.frag"};
    const std::vector<GLuint> shaderTypes = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

    const std::pair<const std::vector<GLuint>, const char* const*> shaders = {shaderTypes, shaderPaths};

    Engine3D::compileShaders(shaders);

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // this->cameraPositionID = glGetUniformLocation(this->programID, "cameraPos");
    this->viewMatrixID = glGetUniformLocation(this->programID, "viewMatrix");
    this->projectionMatrixID = glGetUniformLocation(this->programID, "projectionMatrix");
}

Engine3D::~Engine3D()
{
    glUseProgram(0);
    glDeleteProgram(this->programID);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void Engine3D::addObject(Object* object)
{
    this->objects.push_back(object);
}

void Engine3D::compile_shader(GLuint& shaderId, GLenum shaderType, const char* shaderFilePath)
{
    GLint isCompiled = 0;
    /* Calls the Function that loads the Shader source code from a file */
    const char* shaderSource = get_shader_content(shaderFilePath);

    shaderId = glCreateShader(shaderType);
    if(shaderId == 0) {
        std::printf("COULD NOT LOAD SHADER: %s!\n", shaderFilePath);
    }

    glShaderSource(shaderId, 1, static_cast<const char**>(&shaderSource), nullptr);
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

    if(isCompiled == GL_FALSE) { /* Here You should provide more error details to the User*/
        printf("Shader Compiler Error: %s\n", shaderFilePath);
        glDeleteShader(shaderId);
        return;
    }
}

void Engine3D::compileShaders(const std::vector<GLuint>& shaderTypes, const char* shaderFilePaths[])
{
    for (int i = 0; i < shaderTypes.size(); i++)
    {
        this->shaderIDs.push_back(0);
        compile_shader(shaderIDs[i], shaderTypes[i], shaderFilePaths[i]);
    }

    link_shader();
    glUseProgram(this->programID);
}

void Engine3D::compileShaders(const std::pair<const std::vector<GLuint>, const char* const*>& shaders)
{
    for (int i = 0; i < shaders.first.size(); i++)
    {
        this->shaderIDs.push_back(0);
        compile_shader(shaderIDs[i], shaders.first[i], shaders.second[i]);
    }

    link_shader();
    glUseProgram(this->programID);
}

void Engine3D::createCamera(const ldvec3& position, const ldvec3& front, const ldvec3& upVector)
{
    this->camera = new Camera(position, front, upVector);
}

void Engine3D::createCamera()
{
    this->camera = new Camera({0.0, 0.0, 5.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0});
}


void Engine3D::Debug(const std::vector<ldvec3>& points) const
{
    std::cout << "Debugging points: " << points.size() << std::endl;
    glfwMakeContextCurrent(this->window);
    if (!this->window) return;
    if (!this->camera) return;

    GLfloat* vertices = static_cast<GLfloat*>(malloc(6 * sizeof(GLfloat) * points.size()));

    for (unsigned long long i = 0; i < points.size(); i++)
    {
        const float mod = std::fmod(static_cast<float>(i), 3.f);
        vertices[6 * i + 0] = static_cast<GLfloat>(points[i].x);
        vertices[6 * i + 1] = static_cast<GLfloat>(points[i].y);
        vertices[6 * i + 2] = static_cast<GLfloat>(points[i].z);
        vertices[6 * i + 3] = std::floor(mod) == 0.f ? 1.f : 0.f;
        vertices[6 * i + 4] = std::floor(mod) == 1.f ? 1.f : 0.f;
        vertices[6 * i + 5] = std::floor(mod) == 2.f ? 1.f : 0.f;
    }

    unsigned long long count = 0;
    unsigned long long count_wait = 0;

    while (!glfwWindowShouldClose(this->window))
    {
        if (count_wait == 0)
        {
            count += 3;
        }

        if (count >= points.size() && count_wait >= 0)
        {
            count = 1;
            count_wait = 0;
        }
        else if (count_wait >= 0)
        {
            count_wait = 0;
        }
        else
        {
            count_wait++;
        }

        GLfloat* sliced_vertices = static_cast<GLfloat*>(malloc(6 * sizeof(GLfloat) * count));
        for (unsigned long long i = 0; i < 6 * count; i++)
        {
            sliced_vertices[i] = *(vertices + i);
        }

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput();
        this->camera->processInput(this->window, deltaTime);

        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * count * 6, sliced_vertices, GL_DYNAMIC_DRAW);

        camera->setFront(cameraF);
        glm::mat4 viewMatrix = camera->getViewMatrix();
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), static_cast<long double>(width_) / static_cast<long double>(height_), 0.1L, 100.0L);

        glUniformMatrix4fv(this->viewMatrixID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(this->projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glPointSize(10.0f);
        glDrawArrays(GL_POINTS, 0, 3 * count);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    free(vertices);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Engine3D::Draw()
{
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    processInput();
    this->camera->processInput(this->window, deltaTime);

    // glLoadIdentity();

    camera->setFront(cameraF);
    glm::mat4 viewMatrix = camera->getViewMatrix();
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), static_cast<long double>(width_) / static_cast<long double>(height_), 0.1L, 100.0L);

    glUniformMatrix4fv(this->viewMatrixID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(this->projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glDrawArrays(GL_TRIANGLES, 0, 18 * (res + 1) * (res + 0));

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Engine3D::framebuffer_size_callback(GLFWwindow* /*win*/, const int width, const int height)
{
    glViewport(0, 0, width, height);

    width_ = width;
    height_ = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // bottom-left origin: y down
    glOrtho(0.0, width, 0.0, height, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

Camera* Engine3D::getCamera()
{
    if (!this->camera)
    {
        this->createCamera();
    }
    return this->camera;
}

GLFWwindow* Engine3D::getWindow() const
{
    return this->window;
}

char* Engine3D::get_shader_content(const char* fileName)
{
    long size = 0;

    /* Read File to get size */
    FILE* fp = fopen(fileName, "rb");
    if(fp == nullptr) {
        return nullptr;
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp)+1;
    fclose(fp);

    /* Read File for Content */
    fp = fopen(fileName, "r");
    char* shaderContent = static_cast<char*>(std::memset(malloc(size), '\0', size));
    fread(shaderContent, 1, size-1, fp);
    fclose(fp);

    return shaderContent;
}

void Engine3D::link_shader()
{
    GLint isLinked = 0;
    GLint maxLength = 0;
    char* infoLog= static_cast<char*>(malloc(sizeof(char) * 1024));

    this->programID = glCreateProgram();

    for (const unsigned int shaderID : this->shaderIDs)
    {
        glAttachShader(this->programID, shaderID);
    }

    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE) {
        printf("Shader Program Linker Error\n");

        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
        glGetProgramInfoLog(programID, maxLength, &maxLength, &infoLog[0]);

        printf("%s\n", infoLog);

        glDeleteProgram(programID);

        for (const unsigned int shaderID : this->shaderIDs)
        {
            glDeleteShader(shaderID);
        }

        free(infoLog);

        return;
    }

    for (const unsigned int shaderID : this->shaderIDs)
    {
        glDetachShader(this->programID, shaderID);
        glDeleteShader(shaderID);
    }

    free(infoLog);
}

void Engine3D::mainLoop()
{
    glfwMakeContextCurrent(this->window);
    if (!this->window) return;
    if (!this->camera) return;

    const std::vector<ldvec3> points = this->objects[0]->Construct();

    GLfloat vertices[6 * points.size()];

    for (unsigned long long i = 0; i < points.size(); i++)
    {
        const long double mod = std::fmod(i, 3.0L);
        vertices[6 * i + 0] = points[i].x;
        vertices[6 * i + 1] = points[i].y;
        vertices[6 * i + 2] = points[i].z;
        vertices[6 * i + 3] = std::floor(mod) == 0.f ? 1.f : 0.f;
        vertices[6 * i + 4] = std::floor(mod) == 1.f ? 1.f : 0.f;
        vertices[6 * i + 5] = std::floor(mod) == 2.f ? 1.f : 0.f;
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(this->window))
    {
        Draw();
    }
}

void Engine3D::mouse_callback(GLFWwindow* /*win*/, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    long double xoffset = xpos - lastX;
    long double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const long double sensitivity = 10.0 * deltaTime;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    ldvec3 direction;
    direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    direction.y = std::sin(glm::radians(pitch));
    direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    cameraF = glm::normalize(direction);
}

void Engine3D::scroll_callback(GLFWwindow* /*win*/, double xoffset, double yoffset)
{
    fov -= yoffset;
    if (fov < 1.0)
        fov = 1.0;
    if (fov > 90.0)
        fov = 90.0;
}

void Engine3D::setScreenSize(const int width, const int height)
{
    this->screenWidth = width;
    this->screenHeight = height;
}

void Engine3D::startGLFW()
{
    if (!glfwInit())
    {
        std::cerr << "GLFW failed to initialize\n";
        return;
    }

    // Request a compatibility profile if relying on immediate mode
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    this->window = glfwCreateWindow(screenWidth, screenHeight, "Physics Simulation", nullptr, nullptr);
    if (!this->window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1); // vsync
    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(this->window, mouse_callback);
    glfwSetScrollCallback(this->window, scroll_callback);

    // Initial viewport/projection
    int fbw, fbh;
    glfwGetFramebufferSize(this->window, &fbw, &fbh);
    framebuffer_size_callback(this->window, fbw, fbh);
    //mouse_callback(this->window, lastX, lastY);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    // Optional clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);
}

void Engine3D::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Engine3D::processInput() const
{
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);
}