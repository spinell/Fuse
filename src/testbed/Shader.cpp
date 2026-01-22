#include "Shader.h"

#include "fuse/math/Mat4.h"
#include "fuse/math/Vec4.h"
#include "Log.h"


namespace {
const char* vertex_shader_source = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aUV;

out vec2 uv;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = proj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    uv = aUV;
}
)";

const char* pixel_shader_source = R"(
#version 330 core

in vec2 uv;
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec4 diffuseColor;
uniform vec4 uvScale = vec4(1,1,0,0);

void main()
{
    FragColor = diffuseColor * texture(ourTexture, uv * uvScale.xy + uvScale.zw);
}
)";

static GLuint createShader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        fuse::logError("Shader compilation failed: {}", infoLog);
    }
    return shader;
}

} // namespace


Shader::Shader() {
    GLuint vertexShader   = createShader(vertex_shader_source, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(pixel_shader_source, GL_FRAGMENT_SHADER);

    mProgram = glCreateProgram();
    glAttachShader(mProgram, vertexShader);
    glAttachShader(mProgram, fragmentShader);
    glLinkProgram(mProgram);

    int  linkStatus = 0;
    char infoLog[512]{};
    glGetProgramiv(mProgram, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus) {
        glGetProgramInfoLog(mProgram, 512, nullptr, infoLog);
        fuse::logError("Shader program failed to link : {}", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() { glDeleteProgram(mProgram); }

void Shader::bind() { glUseProgram(mProgram); }

void Shader::unbind() { glUseProgram(0); }

void Shader::setVector(const char* name, const fuse::Vec4& vec) {
    glProgramUniform4f(mProgram, getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::setMatrix(const char* name, const fuse::Mat4& matrix) {

    glProgramUniformMatrix4fv(mProgram, getUniformLocation(name), 1, GL_FALSE, matrix.data());
}

GLint Shader::getUniformLocation(const char* uniformName) const {
    GLint location = glGetUniformLocation(mProgram, uniformName);
    if (location == -1) {
        fuse::logWarn("Could not find uniform '{}' in shader.", uniformName);
    }
    return location;
}
