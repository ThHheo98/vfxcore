#include <iostream>

#include "vfxcore/vec3.h"

int main()
{
    const vfxcore::Vec3 a{1.0F, 2.0F, 3.0F};
    const vfxcore::Vec3 b{4.0F, 5.0F, 6.0F};

    std::cout << "dot = " << vfxcore::dot(a, b) << '\n';
    return 0;
}
