#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <utility>

#include "vfxcore/vec3.h"

namespace
{
struct Vec3Deleter
{
    static inline int deleted = 0;

    void operator()(vfxcore::Vec3* value) const
    {
        ++deleted;
        delete value;
    }
};

using TrackedVec3Owner = std::unique_ptr<vfxcore::Vec3, Vec3Deleter>;

void consumeVec3(std::unique_ptr<vfxcore::Vec3> value)
{
    ASSERT_NE(value, nullptr);
    EXPECT_FLOAT_EQ(value->x, 1.0F);
}
}

TEST(Ownership, FreedAtScopeExit)
{
    Vec3Deleter::deleted = 0;
    {
        TrackedVec3Owner value(new vfxcore::Vec3{1, 2, 3});
        EXPECT_EQ(Vec3Deleter::deleted, 0);
    }

    EXPECT_EQ(Vec3Deleter::deleted, 1);
}

TEST(Ownership, FreedOnException)
{
    Vec3Deleter::deleted = 0;
    try
    {
        TrackedVec3Owner value(new vfxcore::Vec3{1, 2, 3});
        throw std::runtime_error("boom");
    }
    catch (const std::runtime_error&)
    {
    }

    EXPECT_EQ(Vec3Deleter::deleted, 1);
}

TEST(Ownership, MoveTransfers)
{
    auto source = std::make_unique<vfxcore::Vec3>(vfxcore::Vec3{1, 2, 3});
    consumeVec3(std::move(source));

    EXPECT_EQ(source, nullptr);
}
