#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include "vfxcore/mesh.h"

using namespace vfxcore;
using Tri = std::array<int, 3>;

TEST(Obj, ReadsTriangle) {
    std::istringstream in("# a comment\nv 0 0 0\nv 1 0 0\nv 0 1 0\nvn 0 0 1\nf 1 2 3\n");
    TriMesh m = readObj(in);
    ASSERT_EQ(m.V.size(), 3u);
    ASSERT_EQ(m.F.size(), 1u);
    EXPECT_EQ(m.F[0], (Tri{0, 1, 2}));   // 1-based in the file, 0-based here
    EXPECT_DOUBLE_EQ(m.V[1].x(), 1.0);
}

TEST(Obj, QuadBecomesTwoTriangles) {
    std::istringstream in("v 0 0 0\nv 1 0 0\nv 1 1 0\nv 0 1 0\nf 1/1/1 2/2/1 3/3/1 4/4/1\n");
    TriMesh m = readObj(in);
    ASSERT_EQ(m.F.size(), 2u);
    EXPECT_EQ(m.F[1], (Tri{0, 2, 3}));
}

TEST(Obj, NegativeIndicesCountBack) {
    std::istringstream in("v 0 0 0\nv 1 0 0\nv 0 1 0\nf -3 -2 -1\n");
    EXPECT_EQ(readObj(in).F[0], (Tri{0, 1, 2}));
}

TEST(Obj, BadIndexThrows) {
    std::istringstream in("v 0 0 0\nf 1 2 3\n");
    EXPECT_THROW(readObj(in), std::runtime_error);
}

TEST(Obj, RoundTrip) {
    std::istringstream in("v 0 0 0\nv 1 0 0\nv 0 1 0.5\nf 1 2 3\n");
    TriMesh a = readObj(in);
    std::stringstream buf;
    writeObj(buf, a);
    TriMesh b = readObj(buf);
    EXPECT_EQ(a.F, b.F);
    EXPECT_TRUE(a.V[2].isApprox(b.V[2]));
}
