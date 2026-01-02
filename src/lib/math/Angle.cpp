#include "Angle.h"

#include <cmath>

using namespace fuse;

Angle Angle::wrap() const noexcept { return std::fmod(mValue, 360.F); }

Angle Angle::wrapSigned() const noexcept {
#if 1
    float remainder = wrapUnsigned().asDegrees();
    if (remainder > 180) {
        remainder -= 360;
    }
    return Angle(remainder);
#else
    // some test will fail.
    float remainder = std::fmod(mValue + 180.f, 360.F);
    if (remainder < 0.0f) {
        remainder += 360.f;
    }
    remainder -= 180.f;
    return remainder;
#endif
}

Angle Angle::wrapUnsigned() const noexcept {
    const float remainder = std::fmod(mValue, 360.F);
    return remainder < 0 ? remainder + 360.F : remainder;
}
