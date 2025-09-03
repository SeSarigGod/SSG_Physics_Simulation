#ifndef PHYSICS_SIMULATION_DELAUNEYTRIANGULATION_H
#define PHYSICS_SIMULATION_DELAUNEYTRIANGULATION_H
#include <vector>
#include <glm/glm.hpp>

#include "LongDoubleGLMExtension.h"


namespace SSG::DT
{
    long double DelauneyDeterminant(const ldvec2& p1, const ldvec2& p2, const ldvec2& p3, const ldvec2& p4);
    std::vector<ldvec2> BasicDelauneyTriangulation(const std::vector<ldvec2>& points);
    std::vector<ldvec3> BasicDelauneyTriangulation(const std::vector<ldvec3>& points);
}


#endif //PHYSICS_SIMULATION_DELAUNEYTRIANGULATION_H