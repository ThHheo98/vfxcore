#include <gtest/gtest.h>

#include "vfxcore/vec3.h"

TEST(Vec3, Dot)
{
    const vfxcore::Vec3 a{1.0F, 2.0F, 3.0F};
    const vfxcore::Vec3 b{4.0F, 5.0F, 6.0F};

    EXPECT_FLOAT_EQ(vfxcore::dot(a, b), 32.0F);
}
