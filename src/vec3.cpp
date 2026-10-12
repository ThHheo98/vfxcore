#include "vfxcore/vec3.h"

namespace vfxcore
{
float dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
}
