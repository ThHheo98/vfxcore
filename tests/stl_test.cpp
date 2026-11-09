#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <numeric>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#include "vfxcore/vec3.h"

namespace
{
using Edge = std::pair<std::size_t, std::size_t>;
using Triangle = std::array<std::size_t, 3>;

std::set<Edge> makeEdgeSet(const std::vector<Triangle>& triangles)
{
    std::set<Edge> edges;
    for (const Triangle& triangle : triangles)
    {
        for (std::size_t i = 0; i < triangle.size(); ++i)
        {
            const std::size_t a = triangle[i];
            const std::size_t b = triangle[(i + 1) % triangle.size()];
            edges.emplace(std::min(a, b), std::max(a, b));
        }
    }

    return edges;
}

vfxcore::Vec3 centroid(const std::vector<vfxcore::Vec3>& points)
{
    if (points.empty())
    {
        throw std::invalid_argument("centroid requires at least one point");
    }

    const vfxcore::Vec3 sum = std::accumulate(
        points.begin(), points.end(), vfxcore::Vec3{},
        [](vfxcore::Vec3 total, const vfxcore::Vec3& point) {
            total.x += point.x;
            total.y += point.y;
            total.z += point.z;
            return total;
        });

    const float count = static_cast<float>(points.size());
    return {sum.x / count, sum.y / count, sum.z / count};
}
}

TEST(Stl, SortsVec3ByX)
{
    std::vector<vfxcore::Vec3> points{
        {3.0F, 0.0F, 0.0F},
        {1.0F, 0.0F, 0.0F},
        {2.0F, 0.0F, 0.0F},
    };

    std::sort(points.begin(), points.end(),
              [](const auto& a, const auto& b) {
                  return a.x < b.x;
              });

    EXPECT_FLOAT_EQ(points[0].x, 1.0F);
    EXPECT_FLOAT_EQ(points[1].x, 2.0F);
    EXPECT_FLOAT_EQ(points[2].x, 3.0F);
}

TEST(Stl, CalculatesCentroid)
{
    const std::vector<vfxcore::Vec3> points{
        {3.0F, 2.0F, 1.0F},
        {1.0F, 4.0F, 3.0F},
        {2.0F, 6.0F, 5.0F},
    };

    const vfxcore::Vec3 center = centroid(points);

    EXPECT_FLOAT_EQ(center.x, 2.0F);
    EXPECT_FLOAT_EQ(center.y, 4.0F);
    EXPECT_FLOAT_EQ(center.z, 3.0F);
}

TEST(Stl, CentroidOfEmptyInputThrows)
{
    const std::vector<vfxcore::Vec3> points;

    EXPECT_THROW(centroid(points), std::invalid_argument);
}

TEST(Stl, EdgeSetDeduplicatesSharedUndirectedEdges)
{
    const std::vector<Triangle> triangles{
        {0, 1, 2},
        {2, 1, 3},
    };

    const std::set<Edge> edges = makeEdgeSet(triangles);
    const std::set<Edge> expected{
        {0, 1},
        {0, 2},
        {1, 2},
        {1, 3},
        {2, 3},
    };

    EXPECT_EQ(edges, expected);
}
