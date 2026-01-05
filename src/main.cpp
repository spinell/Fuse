#include "imgui.h"
#include "math/Angle.h"
#include "math/Mat4.h"
#include "math/Vec3.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <glad/gl.h>
#include <SDL3/SDL.h>

#include <cassert>
#include <print>
#include <utility>

constexpr int kScreenWidth{640};
constexpr int kScreenHeight{480};

SDL_Window*  gWindow{nullptr};
SDL_Surface* gScreenSurface{nullptr};

const char* vertex_shader_source = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

out vec4 oColor;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = proj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    oColor = aColor;
}
)";

const char* pixel_shader_source = R"(
#version 330 core

in vec4 oColor;
out vec4 FragColor;

uniform vec4 ourColor;

void main()
{
    FragColor = ourColor * oColor;
}
)";

using namespace fuse;

void openglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                         const GLchar* message, const void* userParam) {

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
        };
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
        };
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
        };
    }();

    std::println("[OpenGL][{}][{}][{}]({}) {}", typeStr, sourceStr, severityStr, id, message);
}

static GLuint createShader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::println("Shader compilation failed: {}", infoLog);
    }
    return shader;
}

float vertices[] = {
  // clang-format off
    // back red
    -0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
     0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
     0.5f,  0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
     0.5f,  0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
    -0.5f,  0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
    -0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
    // front green
    -0.5f, -0.5f,  0.5f, 0.f, 1.f, 0.f, 1.f,
     0.5f, -0.5f,  0.5f, 0.f, 1.f, 0.f, 1.f,
     0.5f,  0.5f,  0.5f, 0.f, 1.f, 0.f, 1.f,
     0.5f,  0.5f,  0.5f, 0.f, 1.f, 0.f, 1.f,
    -0.5f,  0.5f,  0.5f, 0.f, 1.f, 0.f, 1.f,
    -0.5f, -0.5f,  0.5f, 0.f, 1.f, 0.f, 1.f,
    // left blue
    -0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f, 1.f,
    -0.5f,  0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f,
    -0.5f, -0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f,
    -0.5f, -0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f,
    -0.5f, -0.5f,  0.5f, 0.f, 0.f, 1.f, 1.f,
    -0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f, 1.f,
    // right yellow
     0.5f,  0.5f,  0.5f, 1.f, 1.f, 0.f, 1.f,
     0.5f,  0.5f, -0.5f, 1.f, 1.f, 0.f, 1.f,
     0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, 1.f,
     0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, 1.f,
     0.5f, -0.5f,  0.5f, 1.f, 1.f, 0.f, 1.f,
     0.5f,  0.5f,  0.5f, 1.f, 1.f, 0.f, 1.f,
    // bottom cyan
    -0.5f, -0.5f, -0.5f, 0.f, 1.f, 1.f, 1.f,
     0.5f, -0.5f, -0.5f, 0.f, 1.f, 1.f, 1.f,
     0.5f, -0.5f,  0.5f, 0.f, 1.f, 1.f, 1.f,
     0.5f, -0.5f,  0.5f, 0.f, 1.f, 1.f, 1.f,
    -0.5f, -0.5f,  0.5f, 0.f, 1.f, 1.f, 1.f,
    -0.5f, -0.5f, -0.5f, 0.f, 1.f, 1.f, 1.f,
    // top purple
    -0.5f,  0.5f, -0.5f, 1.f, 0.f, 1.f, 1.f,
     0.5f,  0.5f, -0.5f, 1.f, 0.f, 1.f, 1.f,
     0.5f,  0.5f,  0.5f, 1.f, 0.f, 1.f, 1.f,
     0.5f,  0.5f,  0.5f, 1.f, 0.f, 1.f, 1.f,
    -0.5f,  0.5f,  0.5f, 1.f, 0.f, 1.f, 1.f,
    -0.5f,  0.5f, -0.5f, 1.f, 0.f, 1.f, 1.f,
  // clang-format on
};

Mat4 projectionMatrix;

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
          SDL_CreateWindow("SDL3 Tutorial: Hello SDL3", kScreenWidth, kScreenHeight, window_flags);
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
    // contextFlag |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
    // contextFlag |= SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG;
    contextFlag |= SDL_GL_CONTEXT_RESET_ISOLATION_FLAG;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_NO_ERROR, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, contextFlag);
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
    for (GLint i = 0; i < nbShaderLang; i++) {
        const GLubyte* version = glGetStringi(GL_SHADING_LANGUAGE_VERSION, i);
        std::println(" - Shader Version: {}", (const char*)version);
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

    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    GLuint vertexShader   = createShader(vertex_shader_source, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(pixel_shader_source, GL_FRAGMENT_SHADER);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int projLocation        = glGetUniformLocation(shaderProgram, "proj");
    int viewLocation        = glGetUniformLocation(shaderProgram, "view");
    int modelLocation       = glGetUniformLocation(shaderProgram, "model");
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);

    const auto transform =
      fuse::Mat4::kIdentity; //fuse::Mat4::CreateRotationZ(fuse::degrees(1) * SDL_GetTicks());
    //glUniformMatrix4fv(viewLocation, 1,  GL_FALSE, fuse::Mat4::CreateTranslation({0,0,-10}).data());
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, transform.data());

    //glUniformMatrix4fv(projLocation, 1,  GL_FALSE, fuse::Mat4::ProjOrtho(10,10).data());

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    projectionMatrix =
      fuse::Mat4::CreateProjectionPerspectiveFOVY(fuse::degrees(45), 800.f / 600.f, 0.01f, 1000.f);

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
                projectionMatrix = fuse::Mat4::CreateProjectionPerspectiveFOVY(fuse::degrees(45),
                                                                               800.f / 600.f,
                                                                               0.01f,
                                                                               1000.f);
            }
            ImGui_ImplSDL3_ProcessEvent(&e);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix.data());
        glUniformMatrix4fv(
          viewLocation,
          1,
          GL_FALSE,
          fuse::Mat4::CreateViewLookAt({0, 0, 20}, {0, 0, -1}, Vec3::kUnitY).data());

        glBindVertexArray(VAO);

        // first cube
        auto transform = fuse::Mat4::CreateRotation(fuse::degrees(35) * SDL_GetTicks() / 1000.f,
                                                    fuse::Vec3(0, 1, 0).normalize());
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, transform.data());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // second cube
        transform = fuse::Mat4::CreateTranslation({-10, 0, 0}) *
                    fuse::Mat4::CreateRotation(fuse::degrees(35) * SDL_GetTicks() / 1000.f,
                                               fuse::Vec3(0, 1, 0).normalize());
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, transform.data());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        const auto transform2 = fuse::Mat4::CreateTranslation({+10, 0, 0});
        transform             = transform * transform2;
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, transform2.data());
        glDrawArrays(GL_TRIANGLES, 0, 36);

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
