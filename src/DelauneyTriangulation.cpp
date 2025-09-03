#include "DelauneyTriangulation.h"

#include <vector>
#include <glm/glm.hpp>

#include "LongDoubleGLMExtension.h"

namespace SSG::DT
{
    long double DelauneyDeterminant(const ldvec2& p1, const ldvec2& p2, const ldvec2& p3, const ldvec2& p4)
    {
        const long double determinant = (p1.x - p4.x) * (-p2.y * p2.y * p3.y + p1.x * p3.y * (p2.x - p4.x) + p2.x * p3.y * (-p2.x + p4.x) +
                                    p1.x * p2.y * (-p3.x + p4.x) + p2.y * (p3.x * p3.x + p3.y * p3.y - p3.x * p4.x)) +
                                    ((p2.y * p2.y - (p1.x - p2.x) * (p2.x - p3.x)) * (p1.x - p3.x) + (-p1.x + p2.x) * p3.y * p3.y)
                                    * p4.y + (p2.y * p3.x - p2.x * p3.y + p1.x * (-p2.y + p3.y)) * p4.y * p4.y + p1.y * p1.y *
                                    (-p3.y * p4.x + p2.y * (-p3.x + p4.x) + p2.x * (p3.y - p4.y) + p3.x * p4.y) + p1.y *
                                    (p2.x * p2.x * (p3.x - p4.x) + p2.y * p2.y * (p3.x - p4.x) + p4.x * (p3.x * p3.x + p3.y * p3.y
                                    - p3.x * p4.x) - p3.x * p4.y * p4.y + p2.x * (-p3.x * p3.x - p3.y * p3.y + p4.x * p4.x + p4.y * p4.y));

        return determinant;
    }

    std::vector<ldvec2> BasicDelauneyTriangulation(const std::vector<ldvec2>& points)
    {
        return points;
    }
    std::vector<ldvec3> BasicDelauneyTriangulation(const std::vector<ldvec3>& points)
    {
        std::vector<ldvec3> triangulation{};



        return triangulation;
    }
}
