#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

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

struct Joint
{
    std::vector<std::shared_ptr<Joint>> children;
    std::weak_ptr<Joint> parent;
};

struct CyclicJoint
{
    std::vector<std::shared_ptr<CyclicJoint>> children;
    std::shared_ptr<CyclicJoint> parent;
};

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

TEST(Ownership, JointHierarchyUsesWeakParentLink)
{
    auto root = std::make_shared<Joint>();
    auto child = std::make_shared<Joint>();
    root->children.push_back(child);
    child->parent = root;

    std::weak_ptr<Joint> weakRoot = root;
    std::weak_ptr<Joint> weakChild = child;

    ASSERT_EQ(child->parent.lock(), root);

    root.reset();

    EXPECT_TRUE(weakRoot.expired());
    EXPECT_FALSE(weakChild.expired());
    EXPECT_TRUE(child->parent.expired());

    child.reset();
    EXPECT_TRUE(weakChild.expired());
}

TEST(Ownership, SharedParentAndChildPointersFormCycle)
{
    auto root = std::make_shared<CyclicJoint>();
    auto child = std::make_shared<CyclicJoint>();
    root->children.push_back(child);
    child->parent = root;

    std::weak_ptr<CyclicJoint> weakRoot = root;
    std::weak_ptr<CyclicJoint> weakChild = child;

    root.reset();
    child.reset();

    EXPECT_FALSE(weakRoot.expired());
    EXPECT_FALSE(weakChild.expired());

    auto retainedChild = weakChild.lock();
    ASSERT_NE(retainedChild, nullptr);
    retainedChild->parent.reset();
    retainedChild.reset();

    EXPECT_TRUE(weakRoot.expired());
    EXPECT_TRUE(weakChild.expired());
}
