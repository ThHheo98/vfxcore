#pragma once
#include <array>
#include <filesystem>
#include <iosfwd>
#include <vector>
#include <Eigen/Core>

namespace vfxcore {

struct TriMesh {
    std::vector<Eigen::Vector3d> V;     // positions
    std::vector<std::array<int, 3>> F;  // triangles, 0-based
};

TriMesh readObj(std::istream& in);
TriMesh readObj(const std::filesystem::path& path);
void writeObj(std::ostream& out, const TriMesh& mesh);
void writeObj(const std::filesystem::path& path, const TriMesh& mesh);

std::vector<std::vector<int>> vertexNeighbours(const TriMesh& mesh);
void laplacianSmooth(TriMesh& mesh, int iterations, double lambda);

}