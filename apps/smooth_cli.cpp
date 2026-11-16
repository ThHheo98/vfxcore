#include <exception>
#include <iostream>
#include <string>
#include "vfxcore/mesh.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "usage: smooth in.obj out.obj [iterations=10] [lambda=0.5]\n";
        return 1;
    }
    try {
        const int iterations = argc > 3 ? std::stoi(argv[3]) : 10;
        const double lambda = argc > 4 ? std::stod(argv[4]) : 0.5;
        auto mesh = vfxcore::readObj(std::filesystem::path(argv[1]));
        std::cout << mesh.V.size() << " vertices, " << mesh.F.size() << " triangles\n";
        vfxcore::laplacianSmooth(mesh, iterations, lambda);
        vfxcore::writeObj(std::filesystem::path(argv[2]), mesh);
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
}