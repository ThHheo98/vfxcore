#include <gtest/gtest.h>

#include "vfxcore/vec3.h"

namespace
{
const vfxcore::Vec3* inspectByConstReference(const vfxcore::Vec3& value)
{
    return &value;
}
}

TEST(Semantics, AssignmentCopies)
{
    const vfxcore::Vec3 a{1, 2, 3};
    vfxcore::Vec3 b = a;
    b.x = 99;

    EXPECT_EQ(a.x, 1);
    EXPECT_EQ(b.x, 99);
}

TEST(Semantics, ReferenceAliases)
{
    vfxcore::Vec3 a{1, 2, 3};
    vfxcore::Vec3& reference = a;
    reference.x = 99;

    EXPECT_EQ(a.x, 99);
}

TEST(Semantics, ConstReferenceUsesOriginalObject)
{
    const vfxcore::Vec3 value{1, 2, 3};

    EXPECT_EQ(inspectByConstReference(value), &value);
}
