#include <vector>

#include "Engine3D.h"


constexpr int screenWidth = 1280;
constexpr int screenHeight = 720;

int main(int /*argc*/, char** /*argv*/)
{
    auto* engine = new Engine3D(screenWidth, screenHeight);

    constexpr double radius[] = {0.5};
    auto* testSphere = new Object({0.0, 0.0, 0.0}, 1.0, ObjectType::SPHERE, radius);

    engine->addObject(testSphere);

    engine->mainLoop();

    return 0;
}
