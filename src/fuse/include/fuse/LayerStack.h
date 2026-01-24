#pragma once
#include "Layer.h"

#include <cassert>
#include <vector>

namespace fuse {

/// @brief
class LayerStack {
public:
    LayerStack() = default;
    ~LayerStack();

    LayerStack(const LayerStack&)            = delete;
    LayerStack(LayerStack&&)                 = delete;
    LayerStack& operator=(const LayerStack&) = delete;
    LayerStack& operator=(LayerStack&&)      = delete;

    void clear();

    /// @brief
    /// @return
    [[nodiscard]] size_t size() const { return mLayers.size(); }

    /// @brief
    /// @param layer
    void pushLayer(Layer* layer);

    /// @brief
    /// @param overlay
    void pushOverlay(Layer* overlay);

    /// @brief
    /// @param layer
    void popLayer(Layer* layer);

    /// @brief
    /// @param overlay
    void popOverlay(Layer* overlay);

    /// @brief
    /// @return
    std::vector<Layer*>::iterator begin() { return mLayers.begin(); }

    std::vector<Layer*>::reverse_iterator rbegin() { return mLayers.rbegin(); }

    /// @brief
    /// @return
    std::vector<Layer*>::iterator end() { return mLayers.end(); }

    std::vector<Layer*>::reverse_iterator rend() { return mLayers.rend(); }

private:
    std::vector<Layer*> mLayers;
    unsigned int        mLayerInsertIndex{};
};

} // namespace fuse
