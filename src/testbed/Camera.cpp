#include "Camera.h"

#include "math/Mat3.h"

#include <algorithm>
#include <cmath>

using namespace fuse;

namespace {
const Vec3 kWorldUp = Vec3::kUnitY;

/// @brief
/// @param theta Angle in y/x plane (pitch)
/// @param phi   Angle in z/x plane (yaw)
/// @return
/// @todo Understand why that work ....
Vec3 makeVec3FromSpherical(Angle pitch, Angle yaw) {
    return {-std::cos(pitch) * std::sin(yaw), std::sin(pitch), -std::cos(pitch) * std::cos(yaw)};
}

} // namespace


Mat4 Camera::getProjectionMatrix() const {
    return Mat4::CreateProjectionPerspectiveFOVY(mFoVy, mAspectRatio, mNearZ, mFarZ);
}


Mat4 Camera::getViewMatrix() const {
    const auto forward = getDirection();
    return Mat4::CreateViewLookTo(mPosition, forward, kWorldUp);
}


fuse::Mat4 Camera::getViewProjectionMatrix() const {
    return getViewMatrix() * getProjectionMatrix();
}

float Camera::getFarWindowHeight() const { return 2.0f * mFarZ * std::tan(0.5f * mFoVy); }
float Camera::getFarWindowWidth() const { return mAspectRatio * getFarWindowHeight(); }
float Camera::getNearWindowHeight() const { return 2.0f * mNearZ * std::tan(0.5f * mFoVy); }
float Camera::getNearWindowWidth() const { return mAspectRatio * getNearWindowHeight(); }


fuse::Angle Camera::getFovY() const { return mFoVy; }

fuse::Angle Camera::getFovX() const {
    const float halfWidth = 0.5f * getNearWindowWidth();
    return radians(2.0f * std::atan(halfWidth / mNearZ));
}

void Camera::setFovY(Angle fov) {
    // clamp the field of view to avoid scene flipping.
    mFoVy = fuse::degrees(std::clamp(fov.asDegrees(), 1.f, 179.f));
}

void Camera::pitch(fuse::Angle pitch) {
    mPitch += pitch;
    // make sure that when pitch is out of bounds, screen doesn't get flipped
    mPitch = fuse::degrees(std::clamp(mPitch.asDegrees(), -89.f, 89.f));
}

void Camera::setPitch(fuse::Angle angle) {
    mPitch = angle;
    mPitch = fuse::degrees(std::clamp(mPitch.asDegrees(), -89.f, 89.f));
}

fuse::Angle Camera::getPitch() const { return mPitch; }

void Camera::yaw(fuse::Angle yaw) { mYaw += yaw; }

void Camera::setYaw(fuse::Angle angle) { mYaw = angle; }

fuse::Angle Camera::getYaw() const { return mYaw; }

Vec3 Camera::getDirection() const { return makeVec3FromSpherical(mPitch, mYaw); }

Vec3 Camera::getRight() const { return getDirection().cross(kWorldUp).normalize(); }

Vec3 Camera::getUp() const { return getRight().cross(getDirection()).normalize(); }

void Camera::moveForward(float distance) { mPosition += getDirection() * distance; }

void Camera::moveRight(float distance) { mPosition += getRight() * distance; }

void Camera::moveUp(float distance, bool world) {
    mPosition += (world ? kWorldUp : getUp()) * distance;
}
