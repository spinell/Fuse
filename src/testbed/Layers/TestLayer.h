#pragma once
#include "../Camera.h"
#include "../Mesh.h"
#include "../Shader.h"
#include "../Texture.h"

#include <fuse/Layer.h>

class TestLayer : public fuse::Layer {
private:
    Camera  camera;
    // FIXME: All OpenGL are leaking
    Texture debugMipmap;
    Texture blackWhiteCheckBoardtexture;
    Texture checkBoardtexture;
    Texture xorTexture;
    Texture brickTexture1;
    Texture brickTexture2;
    Texture brickTexture3;
    Texture brickTexture4;
    Texture brickTexture5;
    Texture brickTexture6;
    Texture grass1;
    Texture grass2;
    Mesh    boxMesh;
    Mesh    gridMesh;
    Mesh    geoSphereMesh;
    Mesh    sphereMesh;
    Mesh    cylinderMesh;
    Shader* shader;

public:
    TestLayer();
    ~TestLayer() override;

    bool onEvent(const SDL_Event& e) override;

    void onUpdate(fuse::Time deltaTime) override;

    void onRender() override;

    void onImGui() override;
};
