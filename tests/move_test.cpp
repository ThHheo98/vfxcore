#include <gtest/gtest.h>

#include <utility>
#include <vector>

#include "vfxcore/vec3.h"

namespace
{
struct CountedValue
{
    static inline int copies = 0;
    static inline int moves = 0;

    explicit CountedValue(int value) : value(value) {}

    CountedValue(const CountedValue& other) : value(other.value)
    {
        ++copies;
    }

    CountedValue(CountedValue&& other) noexcept : value(other.value)
    {
        ++moves;
    }

    int value;
};

void changeFirstX(std::vector<vfxcore::Vec3> points)
{
    points[0].x = 99.0F;
}
}

TEST(MoveSemantics, PassingLvalueByValueCopies)
{
    std::vector<vfxcore::Vec3> source{{1.0F, 2.0F, 3.0F}};

    changeFirstX(source);

    EXPECT_FLOAT_EQ(source[0].x, 1.0F);
}

TEST(MoveSemantics, MovingVectorTransfersItsBuffer)
{
    std::vector<vfxcore::Vec3> source{{1.0F, 2.0F, 3.0F}};
    const vfxcore::Vec3* originalData = source.data();

    std::vector<vfxcore::Vec3> destination = std::move(source);

    ASSERT_FALSE(destination.empty());
    EXPECT_EQ(destination.data(), originalData);
    EXPECT_FLOAT_EQ(destination[0].x, 1.0F);
}

TEST(MoveSemantics, VectorReallocationUsesNoexceptMove)
{
    std::vector<CountedValue> values;
    values.reserve(1);
    values.emplace_back(0);
    while (values.size() < values.capacity())
    {
        values.emplace_back(static_cast<int>(values.size()));
    }

    const int elementsToRelocate = static_cast<int>(values.size());
    CountedValue::copies = 0;
    CountedValue::moves = 0;

    values.emplace_back(99);

    EXPECT_EQ(CountedValue::copies, 0);
    EXPECT_EQ(CountedValue::moves, elementsToRelocate);
}
