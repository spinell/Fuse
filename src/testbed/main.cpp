#include "Buffer.h"
#include "Camera.h"
#include "GeometryGenerator.h"
#include "TextureGenerator.h"
#include "math/Angle.h"
#include "math/Mat4.h"
#include "math/Vec3.h"
#include "Shader.h"
#include "Texture.h"

#include <glad/gl.h>
#include <SDL3/SDL.h>

#include "ImGui.h"
#include <imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl3.h>

#include <cassert>
#include <print>
#include <utility>

constexpr int kScreenWidth{1024};
constexpr int kScreenHeight{768};


Camera       camera;
SDL_Window*  gWindow{nullptr};
SDL_Surface* gScreenSurface{nullptr};

struct Mesh {
    Mesh() = default;
    Buffer  vertexBuffer;
    Buffer  indexBuffer;
    GLsizei nbIndices;

    Mesh(const Mesh&)            = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&)                 = default;
    Mesh& operator=(Mesh&&)      = default;

    static Mesh CreateBox() {
        Mesh              mesh;
        GeometryGenerator geometryGenerator;
        const auto        data = geometryGenerator.createBox(1, 1, 1, 3);
        Upload(data, mesh);
        return mesh;
    }
    static Mesh CreateGrid() {
        Mesh              mesh;
        GeometryGenerator geometryGenerator;
        const auto        data = geometryGenerator.createGrid(10, 10, 10, 10);
        Upload(data, mesh);
        return mesh;
    }
    static Mesh CreateGeoSphere() {
        Mesh              mesh;
        GeometryGenerator geometryGenerator;
        const auto        data = geometryGenerator.createGeoSphere(1, 4);
        Upload(data, mesh);
        return mesh;
    }
    static Mesh CreateSphere() {
        Mesh              mesh;
        GeometryGenerator geometryGenerator;
        const auto        data = geometryGenerator.createSphere(1, 25, 25);
        Upload(data, mesh);
        return mesh;
    }
    static Mesh CreateCylinder() {
        Mesh              mesh;
        GeometryGenerator geometryGenerator;
        const auto        data = geometryGenerator.createCylinder(1, 1, 3, 10, 2);
        Upload(data, mesh);
        return mesh;
    }


    static void Upload(const GeometryGenerator::MeshData& data, Mesh& mesh) {
        mesh.vertexBuffer =
          Buffer((GLsizeiptr)(data.Vertices.size() * sizeof(GeometryGenerator::Vertex)),
                 (void*)data.Vertices.data());
        mesh.indexBuffer =
          Buffer((GLsizeiptr)(data.Indices.size() * sizeof(unsigned)), (void*)data.Indices.data());
        mesh.nbIndices = (GLsizei)data.Indices.size();
    }
};

using namespace fuse;

static void openglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei /*length*/, const GLchar* message,
                                const void* /*userParam*/) {

    // ignore these non-significant error codes
    // 131169 :
    // 131185 : Buffer detailed info: Buffer object # (bound to GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations.
    // 131218 :
    // 131204 :
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) {
        return;
    }

    std::string_view sourceStr = [source]() {
        switch (source) {
                // clang-format off
            case GL_DEBUG_SOURCE_API:             return "API";
            case GL_DEBUG_SOURCE_APPLICATION:     return "App";
            case GL_DEBUG_SOURCE_OTHER:           return "Other";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
            case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
            default: assert(false && "Unhandled value!");
                std::unreachable();
                // clang-format on
        }
    }();

    std::string_view typeStr = [type]() {
        switch (type) {
                // clang-format off
            case GL_DEBUG_TYPE_ERROR:               return "Error";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behavior";
            case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
            case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
            case GL_DEBUG_TYPE_MARKER:              return "Marker";
            case GL_DEBUG_TYPE_PUSH_GROUP:          return "PushGroup";
            case GL_DEBUG_TYPE_POP_GROUP:           return "PopGroup";
            case GL_DEBUG_TYPE_OTHER:               return "Other";
            default: assert(false && "Unhandled value!");
                std::unreachable();
                // clang-format on
        }
    }();

    std::string_view severityStr = [severity]() {
        switch (severity) {
                // clang-format off
            case GL_DEBUG_SEVERITY_LOW:          return "Low";
            case GL_DEBUG_SEVERITY_MEDIUM:       return "Medium";
            case GL_DEBUG_SEVERITY_HIGH:         return "High";
            case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
            default: assert(false && "Unhandled value!");
                std::unreachable();
                // clang-format on
        }
    }();

    std::println("[OpenGL][{}][{}][{}]({}) {}", typeStr, sourceStr, severityStr, id, message);
}


static void renderMesh(const Mesh& mesh) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBuffer.getId());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer.getId());

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GeometryGenerator::Vertex),
                          (void*)offsetof(GeometryGenerator::Vertex, Position));
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GeometryGenerator::Vertex),
                          (void*)offsetof(GeometryGenerator::Vertex, Normal));
    glVertexAttribPointer(2,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GeometryGenerator::Vertex),
                          (void*)offsetof(GeometryGenerator::Vertex, TangentU));
    glVertexAttribPointer(3,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GeometryGenerator::Vertex),
                          (void*)offsetof(GeometryGenerator::Vertex, TexC));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glDrawElements(GL_TRIANGLES, mesh.nbIndices, GL_UNSIGNED_INT, 0);
}


static void onImGuiRender() {
    static bool wireframeEnable = false;
    ImGui::ShowDemoWindow(); // Show demo window! :)

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


int main(int, char**) {
    std::println("SDL compiled version: {}.{}.{}",
                 SDL_MAJOR_VERSION,
                 SDL_MINOR_VERSION,
                 SDL_MICRO_VERSION);
    std::println("SDL runtime  version: {}.{}.{}",
                 SDL_VERSIONNUM_MAJOR(SDL_GetVersion()),
                 SDL_VERSIONNUM_MINOR(SDL_GetVersion()),
                 SDL_VERSIONNUM_MICRO(SDL_GetVersion()));

    std::println("NumVideoDrivers {}", SDL_GetNumVideoDrivers());
    for (auto i = 0; i < SDL_GetNumVideoDrivers(); i++) {
        std::println(" - {}", SDL_GetVideoDriver(i));
    }

    std::println("NumAudioDrivers {}", SDL_GetNumAudioDrivers());
    for (auto i = 0; i < SDL_GetNumAudioDrivers(); i++) {
        std::println(" - {}", SDL_GetAudioDriver(i));
    }

    std::println("NumRenderDrivers {}", SDL_GetNumRenderDrivers());
    for (auto i = 0; i < SDL_GetNumRenderDrivers(); i++) {
        std::println(" - {}", SDL_GetRenderDriver(i));
    }

    std::println("NumGPUDrivers {}", SDL_GetNumGPUDrivers());
    for (auto i = 0; i < SDL_GetNumGPUDrivers(); i++) {
        std::println(" - {}", SDL_GetGPUDriver(i));
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_WindowFlags window_flags =
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    if (gWindow =
          SDL_CreateWindow("FuseTestbed", kScreenWidth, kScreenHeight, window_flags);
        gWindow == nullptr) {
        SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
        return -1;
    } else {
        gScreenSurface = SDL_GetWindowSurface(gWindow);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContextFlag contextFlag = 0;
    contextFlag |= SDL_GL_CONTEXT_DEBUG_FLAG;
    contextFlag |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
    contextFlag |= SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG;
    //contextFlag |= SDL_GL_CONTEXT_RESET_ISOLATION_FLAG; // this cause context creation fail in WSL

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_NO_ERROR, 1); // this cause context creation fail in WSL

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, (int)contextFlag);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GLContext gl_context = SDL_GL_CreateContext(gWindow);
    if (gl_context == nullptr) {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return 1;
    }
    if (!SDL_GL_MakeCurrent(gWindow, gl_context)) {
        printf("Error: SDL_GL_MakeCurrent(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "aaaaaaaaaaaaaaaaaaaaaaaa");
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "cccccccccccccccccc");
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ddddddddddddddddddd");
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "eeeeeeeeeeeeeeeeeeee");
    if(!SDL_GL_SetSwapInterval(0)) {
        printf("Error: SDL_GL_SetSwapInterval(): %s\n", SDL_GetError());
    }

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    printf("Glad Version: %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    const GLubyte* glVersion       = glGetString(GL_VERSION);
    const GLubyte* glVendor        = glGetString(GL_VENDOR);
    const GLubyte* glRenderer      = glGetString(GL_RENDERER);
    const GLubyte* glShaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    // const GLubyte *glExtension = glGetString(GL_EXTENSIONS );
    std::println("OpenGL Version       : {}", (const char*)glVersion);
    std::println("OpenGL vendor        : {}", (const char*)glVendor);
    std::println("OpenGL renderer      : {}", (const char*)glRenderer);
    std::println("OpenGL Shader Version: {}", (const char*)glShaderVersion);
    GLint nbShaderLang{};
    glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &nbShaderLang);
    for (GLuint i = 0; i < (GLuint)nbShaderLang; i++) {
        const GLubyte* shaderVersion = glGetStringi(GL_SHADING_LANGUAGE_VERSION, i);
        std::println(" - Shader Version: {}", (const char*)shaderVersion);
    }

    // std::println("OpenGL Extension     : {}", (const char *)glExtension);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(gWindow, gl_context);
    ImGui_ImplOpenGL3_Init();

    GLint glVersionMajor{};
    GLint glVersionMinor{};
    GLint glExtensionCount{};
    GLint glProgramBinaryFormatCount{};
    GLint glShaderBinaryFormatCount{};
    glGetIntegerv(GL_MAJOR_VERSION, &glVersionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &glVersionMinor);
    glGetIntegerv(GL_NUM_EXTENSIONS, &glExtensionCount);
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &glProgramBinaryFormatCount);
    glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &glShaderBinaryFormatCount);

    int flags{};
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    const bool isDebugContext = (flags & GL_CONTEXT_FLAG_DEBUG_BIT) == GL_CONTEXT_FLAG_DEBUG_BIT;
    const bool isForwardContext =
      (flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT) == GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
    const bool isRobustContext =
      (flags & GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT) == GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT;
    const bool isNoErrorContext =
      (flags & GL_CONTEXT_FLAG_NO_ERROR_BIT) == GL_CONTEXT_FLAG_NO_ERROR_BIT;
    if (isDebugContext) {
        std::println("**** Debug context ***");
    }
    if (isForwardContext) {
        std::println("**** Forward context ***");
    }
    if (isRobustContext) {
        std::println("**** Robust context ***");
    }
    if (isNoErrorContext) {
        std::println("**** No Error context ***");
    }

    if (isDebugContext) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(openglDebugCallback, nullptr /*userParam*/);

        GLuint unusedIds = 0;
        glDebugMessageControl(GL_DONT_CARE /*source*/,
                              GL_DONT_CARE /*type*/,
                              GL_DONT_CARE /*severity*/,
                              0 /*count*/,
                              &unusedIds,
                              true);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    Shader               shader;
    Texture debugMipmap = Texture::CreateDebugWithMipmap();
    Texture blackWhiteCheckBoardtexture = Texture::CreateCheckerboard(64, 64, Texture::Color{0,0,0}, Texture::Color{255,255,255}, 8);
    Texture checkBoardtexture = Texture::CreateCheckerboard(1024, 1024, Texture::Color{255,0,0}, Texture::Color{0,255,255}, 8);
    Texture xorTexture = Texture::Create(TextureGenerator::generateXor(256,256));
    Texture brickTexture1 = Texture::Create(TextureGenerator::generateBrickTexture1(1024,1024));
    Texture brickTexture2 = Texture::Create(TextureGenerator::generateBrickTexture2(1024,1024));
    Texture brickTexture3 = Texture::Create(TextureGenerator::generateBrickTexture3(1024,1024));
    Texture brickTexture4 = Texture::Create(TextureGenerator::generateBrickTexture4(1024,1024));
    Texture brickTexture5 = Texture::Create(TextureGenerator::generateBrickTexture5(512,512));
    Texture brickTexture6 = Texture::Create(TextureGenerator::generateBrickTexture6(256,128));
    Texture grass1 = Texture::Create(TextureGenerator::generateGrass(1024,1024));
    Texture grass2 = Texture::Create(TextureGenerator::generateGrass2(1024,1024));
    //Texture              btickTexture1     = Texture::CreateBrick1();
    //Texture              btickTexture2     = Texture::CreateBrick2();
    //Texture              btickTexture3     = Texture::CreateBrick3();
    //Texture              btickTexture4     = Texture::CreateBrick4(1024, 1024, 40, 20, 5);
    //Texture              grassTexture      = Texture::CreateGrass(1024, 1024);
    //Texture              t1                = Texture::Create(1024, 1024, 1);
    //auto textureData = TextureGenerator::generateBrickImage4(1024,1024, 40, 20, 5);
    //t1.upload(0, 1024, 1024, (void*)textureData.data());
    //textureData = TextureGenerator::generateGrass(512,512);
    //t1.upload(1,  512,  512, (void*)textureData.data());
    //textureData = TextureGenerator::generateBrickImage4(256,256, 40, 20, 5);
    //t1.upload(1,  256,  256, (void*)textureData.data());
    std::vector<Texture*> brickTextures = {
        &brickTexture1, &brickTexture2, &brickTexture3, &brickTexture4, &brickTexture5, &brickTexture6};
    std::vector<Texture*> textures = {
        &debugMipmap,
        &checkBoardtexture,
        &xorTexture,
        &brickTexture1,
        &brickTexture2,
        &brickTexture3,
        &brickTexture4,
        &brickTexture5,
        &brickTexture6,
        &grass1,
        &grass2
    };
    unsigned int cubeTextureID = 0;
    unsigned int floorTextureID = 0;

    auto         boxMesh       = Mesh::CreateBox();
    auto         gridMesh      = Mesh::CreateGrid();
    auto         geoSphereMesh = Mesh::CreateGeoSphere();
    auto         sphereMesh    = Mesh::CreateSphere();
    auto         cylinderMesh  = Mesh::CreateCylinder();
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, boxMesh.vertexBuffer.getId());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxMesh.indexBuffer.getId());


    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            // If event is quit type
            if (e.type == SDL_EVENT_QUIT) {
                // End the main loop
                quit = true;
            }
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
                    floorTextureID = (floorTextureID + 1) % (unsigned)textures.size();
                }
                if (e.key.scancode == SDL_SCANCODE_2) {
                    floorTextureID = floorTextureID == 0u ? (unsigned)textures.size() - 1u : floorTextureID - 1u;
                }
                if (e.key.scancode == SDL_SCANCODE_KP_0) {
                    cubeTextureID = (cubeTextureID + 1) % (unsigned)brickTextures.size();
                }
                if (e.key.scancode == SDL_SCANCODE_KP_1) {
                    cubeTextureID = cubeTextureID == 0u ? (unsigned)brickTextures.size() - 1u : cubeTextureID - 1u;
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

            ImGui_ImplSDL3_ProcessEvent(&e);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        onImGuiRender();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();
        glBindTexture(GL_TEXTURE_2D, textures[floorTextureID]->getId());
        shader.setMatrix("proj", camera.getProjectionMatrix());
        shader.setMatrix("view", camera.getViewMatrix());

        // first grid
        {
            shader.setMatrix("model", fuse::Mat4::CreateScaling({20, 1, 20}));
            shader.setVector("diffuseColor", {1, 1, 1, 1});
            shader.setVector("uvScale", {1, 1, 0, 0});
            renderMesh(gridMesh);
        }

        // Cylinder
        glBindTexture(GL_TEXTURE_2D, blackWhiteCheckBoardtexture.getId());
        const unsigned count   = 10;
        const float    spacing = 5.f;
        for (unsigned int i = 0; i < count; i++) {
            float x = (count / 2.f) * -5.f;
            x += ((float)i * spacing);
            shader.setMatrix("model", fuse::Mat4::CreateTranslation({x, 1, -5}));
            shader.setVector("diffuseColor", {1, 1, 1, 1});
            shader.setVector("uvScale", {1, 1, 0, 0});
            renderMesh(cylinderMesh);
        }

        // Sphere
        {
            glBindTexture(GL_TEXTURE_2D, blackWhiteCheckBoardtexture.getId());
            const auto        t     = fuse::Mat4::CreateTranslation({-10, 2, 0});
            const fuse::Angle angle = fuse::degrees(35) * (float)SDL_GetTicks() / 1000.f;
            const auto r = fuse::Mat4::CreateRotation(angle, fuse::Vec3(0, 1, 0).normalize());
            const auto transform = t * r;
            shader.setMatrix("model", transform);
            shader.setVector("diffuseColor", {1, 1, 1, 1});
            renderMesh(sphereMesh);
        }
        // wall of box
        {
            glBindTexture(GL_TEXTURE_2D, brickTextures[cubeTextureID]->getId());
            auto transform = fuse::Mat4::CreateTranslation({+10, 1, 5});
            shader.setVector("diffuseColor", {1, 1, 1, 1});

            transform *= fuse::Mat4::CreateTranslation({0, 0, 0});
            shader.setMatrix("model", transform);
            renderMesh(boxMesh);

            transform *= fuse::Mat4::CreateTranslation({0, 0, 1});
            shader.setMatrix("model", transform);
            renderMesh(boxMesh);

            transform *= fuse::Mat4::CreateTranslation({0, 0, 1});
            shader.setMatrix("model", transform);
            renderMesh(boxMesh);

            transform *= fuse::Mat4::CreateTranslation({0, 0, 1});
            shader.setMatrix("model", transform);
            renderMesh(boxMesh);


            transform *= fuse::Mat4::CreateTranslation({0, 0, 1});
            shader.setMatrix("model", transform);
            renderMesh(boxMesh);

            transform *= fuse::Mat4::CreateTranslation({0, 0, 1});
            shader.setMatrix("model", transform);
            renderMesh(boxMesh);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(gWindow);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}
