#pragma once
#include "Time.h"

union SDL_Event;

namespace fuse {

class Event;

/// @brief Base class for application layer.
/// Layers are used to separate different states of the application.
/// They can be used for game states, editor layers, etc.
/// They are managed by the LayerStack class.
/// Each layer can handle events, update there state, render and use ImGui.
class Layer {
public:
    /// @brief Default constructor. Does nothing.
    Layer() = default;

    Layer(const Layer&)            = delete;
    Layer(Layer&&)                 = delete;
    Layer& operator=(const Layer&) = delete;
    Layer& operator=(Layer&&)      = delete;

    virtual ~Layer() = default;

    /// @brief Handle an event.
    /// @param event The event to handle.
    /// @return True if the event was handled, false otherwise.
    /// @note Default implementation return false.
    virtual bool onEvent(const SDL_Event&) { return false; }

    /// @brief Call each frame to let the layer update its state.
    /// @param deltaTime Time since last frame in millisecond.
    /// @note Default implementation does nothing.
    virtual void onUpdate(Time /*deltaTime*/) {}

    /// @brief Call each frame to let the layer render its content.
    /// @note Default implementation does nothing.
    /// @note This is different from onImGui which is used to render ImGui content
    virtual void onRender() {}

    /// @brief Call each frame to let the layer render its ImGui content.
    /// @note Default implementation does nothing.
    virtual void onImGui() {}
};

} // namespace fuse
