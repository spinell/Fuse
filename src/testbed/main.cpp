#include "fuse/Application.h"
#include "Layers/TestLayer.h"


int main(int, char**) {
    fuse::Application app;
    auto              layer = new TestLayer();
    app.pushLayer(layer);
    app.run();
    delete layer; // FIXME: All GL ressource will leak because the context is already destroyed
    return 0;
}
