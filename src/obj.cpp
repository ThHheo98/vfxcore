#include "vfxcore/mesh.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace vfxcore {
namespace {

// "7", "7/2", "7//3", "7/2/3" -> 0-based index. Negative counts back from the end.
int parseIndex(const std::string& token, int vertexCount) {
    const int i = std::stoi(token.substr(0, token.find('/')));
    if (i > 0) return i - 1;
    if (i < 0) return vertexCount + i;
    throw std::runtime_error("OBJ face index 0 is invalid");
}

}  // namespace

TriMesh readObj(std::istream& in) {
    TriMesh mesh;
    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        std::istringstream ss(line);
        std::string tag;
        if (!(ss >> tag) || tag[0] == '#') continue;
        if (tag == "v") {
            double x, y, z;
            if (!(ss >> x >> y >> z))
                throw std::runtime_error("bad vertex on line " + std::to_string(lineNo));
            mesh.V.emplace_back(x, y, z);
        } else if (tag == "f") {
            std::vector<int> poly;
            for (std::string tok; ss >> tok;)
                poly.push_back(parseIndex(tok, static_cast<int>(mesh.V.size())));
            if (poly.size() < 3)
                throw std::runtime_error("face with under 3 vertices on line " + std::to_string(lineNo));
            for (std::size_t k = 1; k + 1 < poly.size(); ++k)  // fan quads and n-gons into triangles
                mesh.F.push_back({poly[0], poly[k], poly[k + 1]});
        }
        // vt, vn, o, g, s, usemtl, mtllib: skipped for now
    }
    const int n = static_cast<int>(mesh.V.size());
    for (const auto& f : mesh.F)
        for (int i : f)
            if (i < 0 || i >= n) throw std::runtime_error("face index out of range");
    return mesh;
}

TriMesh readObj(const std::filesystem::path& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("can't open " + path.string());
    return readObj(in);
}

void writeObj(std::ostream& out, const TriMesh& mesh) {
    out.precision(9);
    for (const auto& v : mesh.V) out << "v " << v.x() << ' ' << v.y() << ' ' << v.z() << '\n';
    for (const auto& f : mesh.F) out << "f " << f[0] + 1 << ' ' << f[1] + 1 << ' ' << f[2] + 1 << '\n';
}

void writeObj(const std::filesystem::path& path, const TriMesh& mesh) {
    std::ofstream out(path);
    if (!out) throw std::runtime_error("can't write " + path.string());
    writeObj(out, mesh);
}

}