#pragma once
#include "Time.h"

union SDL_Event;

namespace fuse {
class Layer;

/// @brief Base class for fuse application.
class Application {
public:
    /// @brief
    Application();

    /// @brief
    virtual ~Application();

    Application(const Application&)             = delete;
    Application(const Application&&)            = delete;
    Application& operator=(const Application&)  = delete;
    Application& operator=(const Application&&) = delete;

    /// @brief Return the single instance of the application.
    static Application* Get() { return sInstance; }

    /// @brief Start the application and the main loop.
    void run();

    void* getWindow();
    void* getGLContext();

    void pushLayer(Layer* layer);

protected:

    /// @brief Call once every frame to update states.
    /// @param deltaTime The delta time since the last update.
    virtual void onUpdate(Time deltaTime);

    /// @brief Call once every frame for rendering.
    virtual void onRender();

    /// @brief Call once every frame for ImGui.
    virtual void onImGui();

    /// @brief Call when a event happen.
    virtual void onEvent(const SDL_Event&);

private:
    static Application* sInstance;
};

} // namespace fuse
