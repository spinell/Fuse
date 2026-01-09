#pragma once
#include "math/Angle.h"
#include "math/Mat4.h"

/// @brief FlyWay Camera
///
/// The camera use a right hand coordinate system.
/// The camera look at @b negative z-axis.
class Camera {
public:
    Camera() = default;

    /// @brief Set the camera aspect ratio
    /// @param aspectRatio the camera aspect ratio.
    void setAspectRatio(float aspectRatio) { mAspectRatio = aspectRatio; }

    /// @brief Return the camera aspect ratio.
    [[nodiscard]] float getAspectRatio() const { return mAspectRatio; }

    /// @brief Set the camera vertical field of view.
    /// @param fov The vertical field of view angle.
    void setFovY(fuse::Angle fov);

    /// @brief Return the camera vertical field of view.
    [[nodiscard]] fuse::Angle getFovY() const;

    /// @brief Return the camera horizontal field of view.
    [[nodiscard]] fuse::Angle getFovX() const;

    /// @brief Set the camera near plan distance.
    /// @param zNear The distance of the near plan.
    void setZNear(float zNear) { mNearZ = zNear; }

    /// Return the camera distance of the near plan.
    [[nodiscard]] float getZNear() const { return mNearZ; }

    /// @brief Set the camera fqr plan distance.
    /// @param zFqr The distance of the fqr plan.
    void setZFar(float zFar) { mFarZ = zFar; }

    /// Return the camera distance of the near plan.
    [[nodiscard]] float getZFar() const { return mFarZ; }

    [[nodiscard]] float getFarWindowHeight() const;
    [[nodiscard]] float getFarWindowWidth() const;
    [[nodiscard]] float getNearWindowHeight() const;
    [[nodiscard]] float getNearWindowWidth() const;

    /// @brief Set the camera pitch (local rotation around right axis)
    /// @param angle The angle of rotation.
    void setPitch(fuse::Angle angle);

    /// @brief Apply a rotation around local right axis.
    /// @param offset The amount of rotation.
    void pitch(fuse::Angle offset);

    /// @brief Set the camera yaw (rotation around world up axis)
    /// @param angle The angle of rotation.
    void setYaw(fuse::Angle angle);

    /// @brief Apply a rotation around world up axis.
    /// @param offset The amount of rotation.
    void yaw(fuse::Angle offset);

    [[nodiscard]] fuse::Angle getPitch() const;
    [[nodiscard]] fuse::Angle getYaw() const;
    [[nodiscard]] fuse::Vec3  getDirection() const;
    [[nodiscard]] fuse::Vec3  getRight() const;
    [[nodiscard]] fuse::Vec3  getUp() const;
    [[nodiscard]] fuse::Vec3  getPosition() const { return mPosition; }

    [[nodiscard]] fuse::Mat4 getProjectionMatrix() const;
    [[nodiscard]] fuse::Mat4 getViewMatrix() const;
    [[nodiscard]] fuse::Mat4 getViewProjectionMatrix() const;

    /// @brief Set the camera world position
    /// @param position The camera world position.
    void setPosition(const fuse::Vec3& position) { mPosition = position; }

    /// @brief Move the camera in the direction it's looking at.
    /// @param distance The amount of translation.
    ///                 A positive value means forward.
    //                  A negative value means backward.
    void moveForward(float distance);

    /// @brief Move the camera on it's local right axis.
    /// @param distance The amount of translation.
    ///                 A positive value means right.
    //                  A negative value means left.
    void moveRight(float distance);

    /// @brief Move the camera up or down.
    /// @param distance The amount of translation.
    ///                 A positive value means up.
    ///                 A negative value means down.
    /// @param world If true, the translation is done on the world up axis.
    ///              If false, the translation is done on the camera local up vector.
    void moveUp(float distance, bool world = true);

private:
    float       mNearZ       = .1f;
    float       mFarZ        = 1000.f;
    fuse::Angle mFoVy        = fuse::degrees(45.f);
    float       mAspectRatio = 16.f / 9.f;
    fuse::Vec3  mPosition{0, 1, 10}; ///! The camera world position.
    fuse::Angle mYaw{};              ///! The camera rotation around the world up axis.
    fuse::Angle mPitch{};            ///! The camera rotation around it's local right axis.
};
