#include <gtest/gtest.h>
#include "vfxcore/mesh.h"

using namespace vfxcore;

namespace {

// n x n vertices on the XY plane, two triangles per cell, z set per vertex
TriMesh grid(int n, double (*z)(int, int)) {
    TriMesh m;
    for (int y = 0; y < n; ++y)
        for (int x = 0; x < n; ++x) m.V.emplace_back(x, y, z(x, y));
    auto id = [n](int x, int y) { return y * n + x; };
    for (int y = 0; y + 1 < n; ++y)
        for (int x = 0; x + 1 < n; ++x) {
            m.F.push_back({id(x, y), id(x + 1, y), id(x + 1, y + 1)});
            m.F.push_back({id(x, y), id(x + 1, y + 1), id(x, y + 1)});
        }
    return m;
}

double flat(int, int) { return 0.0; }
double bumpy(int x, int y) { return ((x * 7 + y * 13) % 5 - 2) * 0.1; }  // repeatable noise

double roughness(const TriMesh& m) {  // mean squared height
    double s = 0;
    for (const auto& v : m.V) s += v.z() * v.z();
    return s / m.V.size();
}

}  // namespace

TEST(Smooth, NeighboursOnGrid) {
    const auto nbrs = vertexNeighbours(grid(3, flat));
    EXPECT_EQ(nbrs[4].size(), 6u);  // centre of a 3x3 grid
    EXPECT_EQ(nbrs[0].size(), 3u);  // corner on the diagonal
    EXPECT_EQ(nbrs[2].size(), 2u);  // corner off the diagonal
}

TEST(Smooth, CentreOfFlatGridStaysPut) {
    TriMesh m = grid(5, flat);
    const Eigen::Vector3d before = m.V[12];
    laplacianSmooth(m, 1, 0.5);
    EXPECT_TRUE(m.V[12].isApprox(before));  // already at its neighbours' average
}

TEST(Smooth, RemovesNoise) {
    TriMesh m = grid(20, bumpy);
    const double before = roughness(m);
    laplacianSmooth(m, 10, 0.5);
    EXPECT_LT(roughness(m), 0.25 * before);
}

TEST(Smooth, ShrinksOpenBoundary) {  // the known flaw of uniform Laplacian smoothing
    TriMesh m = grid(5, flat);
    laplacianSmooth(m, 20, 0.5);
    EXPECT_GT(m.V[0].x(), 0.1);  // the corner has been pulled inward
}