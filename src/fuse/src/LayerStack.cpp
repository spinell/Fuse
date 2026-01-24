#include "fuse/LayerStack.h"

#include <algorithm>

namespace fuse {

LayerStack::~LayerStack() { clear(); }

void LayerStack::clear() {
    for (Layer* layer : mLayers) {
        delete layer;
    }
}

void LayerStack::pushLayer(Layer* layer) {
    mLayers.emplace(mLayers.begin() + mLayerInsertIndex, layer);
    mLayerInsertIndex++;
}

void LayerStack::pushOverlay(Layer* overlay) { mLayers.emplace_back(overlay); }

void LayerStack::popLayer(Layer* layer) {
    if (auto it = std::ranges::find(mLayers, layer); it != mLayers.end()) {
        mLayers.erase(it);
        --mLayerInsertIndex;
    }
}

void LayerStack::popOverlay(Layer* overlay) {
    if (auto it = std::ranges::find(mLayers, overlay); it != mLayers.end()) {
        mLayers.erase(it);
    }
}

} // namespace fuse
