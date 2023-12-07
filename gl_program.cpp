#include "pch.h"
#include "gl_program.h"
#include "gl_shader.h"

Wrapper::Program::Program(const std::vector<Shader*>& shaders)
{
    LinkShaders(shaders);
}

Wrapper::Program::~Program()
{
    if (m_program) {
        glDeleteProgram(m_program);
        SPDLOG_INFO("Destroy OpenGL Program");
    }
}

void Wrapper::Program::LinkShaders(const std::vector<Shader*>& shaders)
{
    m_program = glCreateProgram();
    SPDLOG_INFO("Create OpenGL Program");

    for (auto& shader : shaders) {
        glAttachShader(m_program, shader->GetShader());
    }

    glLinkProgram(m_program);

    int success = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to link program: {}", infoLog);
        throw std::runtime_error("Failed to link program");
    }

    for (auto& shader : shaders) {
        glDetachShader(m_program, shader->GetShader());
    }
}
