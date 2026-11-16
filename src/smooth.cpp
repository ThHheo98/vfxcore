#include "vfxcore/mesh.h"
#include <algorithm>

namespace vfxcore {

std::vector<std::vector<int>> vertexNeighbours(const TriMesh& mesh) {
    std::vector<std::vector<int>> nbrs(mesh.V.size());
    for (const auto& f : mesh.F)
        for (int k = 0; k < 3; ++k) {
            const int a = f[k], b = f[(k + 1) % 3];
            nbrs[a].push_back(b);
            nbrs[b].push_back(a);
        }
    for (auto& n : nbrs) {  // a shared edge shows up twice: dedupe
        std::sort(n.begin(), n.end());
        n.erase(std::unique(n.begin(), n.end()), n.end());
    }
    return nbrs;
}

void laplacianSmooth(TriMesh& mesh, int iterations, double lambda) {
    const auto nbrs = vertexNeighbours(mesh);
    std::vector<Eigen::Vector3d> next(mesh.V.size());
    for (int it = 0; it < iterations; ++it) {
        for (std::size_t i = 0; i < mesh.V.size(); ++i) {
            if (nbrs[i].empty()) { next[i] = mesh.V[i]; continue; }
            Eigen::Vector3d avg = Eigen::Vector3d::Zero();
            for (int j : nbrs[i]) avg += mesh.V[j];
            avg /= static_cast<double>(nbrs[i].size());
            next[i] = mesh.V[i] + lambda * (avg - mesh.V[i]);
        }
        mesh.V.swap(next);  // Jacobi: every vertex read last pass's positions
    }
}

}