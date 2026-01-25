#include "fuse/Application.h"

#include "fuse/LayerStack.h"
#include "fuse/Timer.h"

#include <glad/gl.h>
#include <SDL3/SDL.h>

#include <imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl3.h>

#include <cassert>
#include <print>
#include <utility>


namespace {
SDL_Window*      window{};
SDL_GLContext    glContext{};
fuse::LayerStack layerStack{};


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


} // namespace


namespace fuse {

Application* Application::sInstance{};

Application::Application() {
    assert(sInstance == nullptr && "Multiple instance not allowed.");
    sInstance = this;

    //
    // Init SDL
    //
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
        //fuse::logFatal("SDL could not initialize! SDL error: {}", SDL_GetError());
    }

    std::println("SDL compiled version: {}.{}.{}",
                 SDL_MAJOR_VERSION,
                 SDL_MINOR_VERSION,
                 SDL_MICRO_VERSION);
    std::println("SDL runtime  version: {}.{}.{}",
                 SDL_VERSIONNUM_MAJOR(SDL_GetVersion()),
                 SDL_VERSIONNUM_MINOR(SDL_GetVersion()),
                 SDL_VERSIONNUM_MICRO(SDL_GetVersion()));

    //
    // Create the main windows.
    //
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 0);
    SDL_GL_SetAttribute(SDL_GL_FLOATBUFFERS, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags windowFlags = 0;
    windowFlags |= SDL_WINDOW_OPENGL;
    windowFlags |= SDL_WINDOW_RESIZABLE;
    windowFlags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;
    window = SDL_CreateWindow("Fuse", 1920, 1080, windowFlags);
    if (window == nullptr) {
        //fuse::logFatal("Window could not be created! SDL error: {}", SDL_GetError());
    }

    //
    // Init OpenGL
    //
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
    glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        std::println("Unable to create OpenGLcontext: {}", SDL_GetError());
    }
    if (!SDL_GL_MakeCurrent(window, glContext)) {
        std::println("Unable to bind OpenGL context: {}", SDL_GetError());
    }

    if (!SDL_GL_SetSwapInterval(0)) {
        std::println("Unable to set OpenGL Swap Interval: {}", SDL_GetError());
    }

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    std::println("Glad Version: {}.{}\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

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

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // IF using Docking Branch

    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init();
}

Application::~Application() = default;

void* Application::getWindow() { return window; }

void* Application::getGLContext() { return glContext; }

void Application::pushLayer(Layer* layer) { layerStack.pushLayer(layer); }

void Application::run() {

    //
    // Main Loop
    //
    fuse::Timer timer;
    timer.reset();
    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            onEvent(e);
        }
        timer.tick();

        // FIXME: type conversion
        onUpdate((double)timer.deltaTime());

        onRender();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        onImGui();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    //
    // Shutdown
    //
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Application::onUpdate(Time deltaTime) {
    for (Layer* layer : layerStack) {
        layer->onUpdate(deltaTime);
    }
}

void Application::onRender() {
    for (Layer* layer : layerStack) {
        layer->onRender();
    }
}

void Application::onImGui() {
    for (Layer* layer : layerStack) {
        layer->onImGui();
    }
}

void Application::onEvent(const SDL_Event& e) {
    for (auto it = layerStack.rbegin(); it != layerStack.rend(); ++it) {
        if ((*it)->onEvent(e)) {
            // The layer has handle the event,
            // stop dispatching the event to other layers.
            break;
        }
    }
}

} // namespace fuse
