#include <vector>

#include "DelauneyTriangulation.h"
#include "Engine3D.h"


constexpr int screenWidth = 2160;
constexpr int screenHeight = 1440;

int main(int /*argc*/, char** /*argv*/)
{
    auto* engine = new SSG::PhysicsEngine::Engine3D(screenWidth, screenHeight);

    constexpr long double radius[] = {0.5L};
    auto* testSphere = new SSG::PhysicsEngine::Object({0.0L, 0.0L, 0.0L}, 1.0L, SSG::PhysicsEngine::ObjectType::SPHERE, radius);

    engine->addObject(testSphere);

    const auto spherePoints = SSG::PhysicsEngine::Object::ConstructModifiedFibonacciSphere();
    std::vector<ldvec3> allPoints = spherePoints;
    std::vector<ldvec3> stereoProjectionPoints{};
    std::vector<ldvec3> stereoSpherePoints{};
    for (auto& point : spherePoints)
    {
        const ldvec2 stereoProjection = SSG::PhysicsEngine::Object::StereoProjection(point);
        allPoints.emplace_back(stereoProjection.x, stereoProjection.y, 0.0L);
        stereoProjectionPoints.emplace_back(stereoProjection.x, stereoProjection.y, 0.0L);
        stereoSpherePoints.push_back(SSG::PhysicsEngine::Object::InverseStereoProjection(stereoProjection));
    }

    // engine->Debug(SSG::DT::BasicDelauneyTriangulation(FlatFibSpherePoints));
    engine->Debug(spherePoints);

    return 0;
}
