#pragma once

namespace vfxcore
{
struct Vec3
{
    float x{};
    float y{};
    float z{};
};

float dot(const Vec3& a, const Vec3& b);
}
