cmake_minimum_required(VERSION 3.28)
project(physics_simulation)

set(CMAKE_CXX_STANDARD 20)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O2")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(physics_simulation main.cpp
        src/Object.cpp
        src/Camera.cpp
        src/Engine3D.cpp
        # shaders/basic.frag
        # shaders/basic.vert
        ${HEADERS}
)

target_include_directories(physics_simulation
    PRIVATE
        ${CMAKE_CURRRENT_SOURCE_DIR}/shaders
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(physics_simulation opengl32 glfw glew32)
