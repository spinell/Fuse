#include "TestLayer.h"
#include "../ImGui.h"
#include "../TextureGenerator.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <imgui.h>

static void onImGuiRender(Camera camera ) {
    static bool wireframeEnable = false;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("Debug info",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Checkbox("Wireframe", &wireframeEnable)) {
        glPolygonMode(GL_FRONT_AND_BACK, wireframeEnable ? GL_LINE : GL_FILL);
    }

    bool isSRgb = glIsEnabled(GL_FRAMEBUFFER_SRGB);
    if (ImGui::Checkbox("SRGB", &isSRgb)) {
        if (isSRgb) {
            glEnable(GL_FRAMEBUFFER_SRGB);
        } else {
            glDisable(GL_FRAMEBUFFER_SRGB);
        }
    }
    bool isDepthTest = glIsEnabled(GL_DEPTH_TEST);
    if (ImGui::Checkbox("DepthTest", &isDepthTest)) {
        if (isDepthTest) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }
    bool isMSAA = glIsEnabled(GL_MULTISAMPLE);
    if (ImGui::Checkbox("MSAA", &isMSAA)) {
        if (isMSAA) {
            glEnable(GL_MULTISAMPLE);
        } else {
            glDisable(GL_MULTISAMPLE);
        }
    }
    fuse::Imgui::TextFmt("{:.3f} ms/frame", 1000.0f / ImGui::GetIO().Framerate);
    fuse::Imgui::TextFmt("{:.1f} FPS", ImGui::GetIO().Framerate);
    ImGui::Separator();
    fuse::Imgui::TextFmt("Fov          => {:.2f} / {:.2f}", camera.getFovY(), camera.getFovX());
    fuse::Imgui::TextFmt("Aspect Ratio => {:.2f}", camera.getAspectRatio());
    fuse::Imgui::TextFmt("Z Plane => {} | {}", camera.getZNear(), camera.getZFar());
    fuse::Imgui::TextFmt("Near  Window => {:.4f} x {:.4f}",
                         camera.getNearWindowWidth(),
                         camera.getNearWindowHeight());
    fuse::Imgui::TextFmt("Far   Window => {:.4f} x {:.4f}",
                         camera.getFarWindowWidth(),
                         camera.getFarWindowHeight());
    ImGui::Separator();
    fuse::Imgui::TextFmt("Direction : {: .5f}", camera.getDirection());
    fuse::Imgui::TextFmt("Right     : {: .5f}", camera.getRight());
    fuse::Imgui::TextFmt("Up        : {: .5f}", camera.getUp());
    auto pos = camera.getPosition();
    if (fuse::Imgui::dragVec3("Position  : ", pos)) {
        camera.setPosition(pos);
    }
    auto pitch = camera.getPitch();
    if (fuse::Imgui::dragAngle("pitch    : ", pitch)) {
        camera.setPitch(pitch);
    }
    auto yaw = camera.getYaw();
    if (fuse::Imgui::dragAngle("yaw      : ", yaw)) {
        camera.setYaw(yaw);
    }

    fuse::Imgui::draw(camera.getViewMatrix());
    fuse::Imgui::draw(camera.getProjectionMatrix());

    ImGui::End();

    ImGui::PopStyleVar(1);
}




TestLayer::TestLayer() {
    shader = new Shader();
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    debugMipmap                 = Texture::CreateDebugWithMipmap();
    blackWhiteCheckBoardtexture = Texture::CreateCheckerboard(64,
                                                              64,
                                                              Texture::Color{0, 0, 0},
                                                              Texture::Color{255, 255, 255},
                                                              8);
    checkBoardtexture           = Texture::CreateCheckerboard(1024,
                                                    1024,
                                                    Texture::Color{255, 0, 0},
                                                    Texture::Color{0, 255, 255},
                                                    8);
    xorTexture                  = Texture::Create(TextureGenerator::generateXor(256, 256));
    brickTexture1 = Texture::Create(TextureGenerator::generateBrickTexture1(1024, 1024));
    brickTexture2 = Texture::Create(TextureGenerator::generateBrickTexture2(1024, 1024));
    brickTexture3 = Texture::Create(TextureGenerator::generateBrickTexture3(1024, 1024));
    brickTexture4 = Texture::Create(TextureGenerator::generateBrickTexture4(1024, 1024));
    brickTexture5 = Texture::Create(TextureGenerator::generateBrickTexture5(512, 512));
    brickTexture6 = Texture::Create(TextureGenerator::generateBrickTexture6(256, 128));
    grass1        = Texture::Create(TextureGenerator::generateGrass(1024, 1024));
    grass2        = Texture::Create(TextureGenerator::generateGrass2(1024, 1024));

    boxMesh       = Mesh::CreateBox();
    gridMesh      = Mesh::CreateGrid();
    geoSphereMesh = Mesh::CreateGeoSphere();
    sphereMesh    = Mesh::CreateSphere();
    cylinderMesh  = Mesh::CreateCylinder();
}


TestLayer::~TestLayer() {
    delete shader;
}

bool TestLayer::onEvent(const SDL_Event& e) {
    if (e.type == SDL_EVENT_WINDOW_RESIZED) {
        const auto width  = e.window.data1;
        const auto height = e.window.data2;
        glViewport(0, 0, width, height);
        camera.setAspectRatio((float)width / (float)height);
    }
    if (e.type == SDL_EVENT_MOUSE_WHEEL) {
        camera.setFovY(camera.getFovY() + fuse::degrees(e.wheel.y * 10));
    }
    if (e.type == SDL_EVENT_MOUSE_MOTION) {
        const bool isLeftDown = (e.motion.state & SDL_BUTTON_LMASK) == SDL_BUTTON_LMASK;
        if (!ImGui::GetIO().WantCaptureMouse && isLeftDown) {
            const auto yaw   = e.motion.xrel * fuse::degrees(0.125f);
            const auto pitch = e.motion.yrel * fuse::degrees(0.125f);
            camera.yaw(-yaw);
            camera.pitch(-pitch);
        }
    }
    if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.scancode == SDL_SCANCODE_1) {
            //floorTextureID = (floorTextureID + 1) % (unsigned)textures.size();
        }
        if (e.key.scancode == SDL_SCANCODE_2) {
            //floorTextureID =
            //  floorTextureID == 0u ? (unsigned)textures.size() - 1u : floorTextureID - 1u;
        }
        if (e.key.scancode == SDL_SCANCODE_KP_0) {
            //cubeTextureID = (cubeTextureID + 1) % (unsigned)brickTextures.size();
        }
        if (e.key.scancode == SDL_SCANCODE_KP_1) {
            //cubeTextureID = cubeTextureID == 0u ? (unsigned)brickTextures.size() - 1u
            //                                    : cubeTextureID - 1u;
        }
        if (e.key.scancode == SDL_SCANCODE_W) {
            camera.moveForward(1.f);
        }
        if (e.key.scancode == SDL_SCANCODE_S) {
            camera.moveForward(-1.f);
        }
        if (e.key.scancode == SDL_SCANCODE_D) {
            camera.moveRight(1.f);
        }
        if (e.key.scancode == SDL_SCANCODE_A) {
            camera.moveRight(-1.f);
        }
        if (e.key.scancode == SDL_SCANCODE_Q) {
            camera.moveUp(1.f);
        }
        if (e.key.scancode == SDL_SCANCODE_E) {
            camera.moveUp(-1.f);
        }
        if (e.key.scancode == SDL_SCANCODE_Z) {
            camera.moveUp(1.f, false);
        }
        if (e.key.scancode == SDL_SCANCODE_X) {
            camera.moveUp(-1.f, false);
        }
    }

    return false;
}

void TestLayer::onUpdate(fuse::Time /*deltaTime*/) {}

void TestLayer::onRender() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FRAMEBUFFER_SRGB);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->bind();
    glBindTexture(GL_TEXTURE_2D, brickTexture1.getId());
    shader->setMatrix("proj", camera.getProjectionMatrix());
    shader->setMatrix("view", camera.getViewMatrix());

    // first grid
    {
        shader->setMatrix("model", fuse::Mat4::CreateScaling({20, 1, 20}));
        shader->setVector("diffuseColor", {1, 1, 1, 1});
        shader->setVector("uvScale", {1, 1, 0, 0});
        gridMesh.render();
    }

    // Cylinder
    glBindTexture(GL_TEXTURE_2D, blackWhiteCheckBoardtexture.getId());
    const unsigned count   = 10;
    const float    spacing = 5.f;
    for (unsigned int i = 0; i < count; i++) {
        float x = (count / 2.f) * -5.f;
        x += ((float)i * spacing);
        shader->setMatrix("model", fuse::Mat4::CreateTranslation({x, 1, -5}));
        shader->setVector("diffuseColor", {1, 1, 1, 1});
        shader->setVector("uvScale", {1, 1, 0, 0});
        cylinderMesh.render();
    }

    // Sphere
    {
        glBindTexture(GL_TEXTURE_2D, blackWhiteCheckBoardtexture.getId());
        const auto        t     = fuse::Mat4::CreateTranslation({-10, 2, 0});
        const fuse::Angle angle = fuse::degrees(35) * (float)SDL_GetTicks() / 1000.f;
        const auto        r = fuse::Mat4::CreateRotation(angle, fuse::Vec3(0, 1, 0).normalize());
        const auto        transform = t * r;
        shader->setMatrix("model", transform);
        shader->setVector("diffuseColor", {1, 1, 1, 1});
        sphereMesh.render();
    }
    // wall of box
    {
        glBindTexture(GL_TEXTURE_2D, brickTexture4.getId());
        auto transform = fuse::Mat4::CreateTranslation({+10, 1, 5});
        shader->setVector("diffuseColor", {1, 1, 1, 1});

        transform *= fuse::Mat4::CreateTranslation({0, 0, 0});
        shader->setMatrix("model", transform);
        boxMesh.render();

        transform *= fuse::Mat4::CreateTranslation({0, 0, 1});
        shader->setMatrix("model", transform);
        boxMesh.render();

        transform *= fuse::Mat4::CreateTranslation({0, 0, 1});
        shader->setMatrix("model", transform);
        boxMesh.render();

        transform *= fuse::Mat4::CreateTranslation({0, 0, 1});
        shader->setMatrix("model", transform);
        boxMesh.render();


        transform *= fuse::Mat4::CreateTranslation({0, 0, 1});
        shader->setMatrix("model", transform);
        boxMesh.render();

        transform *= fuse::Mat4::CreateTranslation({0, 0, 1});
        shader->setMatrix("model", transform);
        boxMesh.render();
    }
}

void TestLayer::onImGui() {
    ImGui::ShowDemoWindow();
    onImGuiRender(camera);
}
