#include "pch.h"
#include "gl_shader.h"
#include "utils.h"

Wrapper::Shader::Shader(const std::string& filename, GLenum shaderType)
{
    CreateShader(filename, shaderType);
}

Wrapper::Shader::~Shader()
{
    if (m_shader) {
        SPDLOG_INFO("Destroy OpenGL Shader - {}", m_shader);
        glDeleteShader(m_shader);
    }
}

void Wrapper::Shader::CreateShader(const std::string& filename, GLenum shaderType)
{
    const auto& code = Utils::LoadTextFile(filename);
    const char* codePtr = code.c_str();
    GLint codeLength = static_cast<GLint>(code.length());

    m_shader = glCreateShader(shaderType);
    glShaderSource(m_shader, 1, (const GLchar* const*)&codePtr, &codeLength);
    glCompileShader(m_shader);

    int result = 0;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &result);

    if (!result) {
        char infoLog[1024];
        glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
        SPDLOG_ERROR("reason: {}", infoLog);
        throw std::runtime_error("Failed to compile shader : " + filename);
    }

    SPDLOG_INFO("Create OpenGL Shader - {} : {}", m_shader, filename);
}
