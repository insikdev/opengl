#include "pch.h"
#include "gl_program.h"
#include "gl_shader.h"

Wrapper::Program::Program(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    auto p_vertexShader = new Wrapper::Shader { vertexShaderFilename, GL_VERTEX_SHADER };
    auto p_fragmentShader = new Wrapper::Shader { fragmentShaderFilename, GL_FRAGMENT_SHADER };

    LinkShaders({ p_vertexShader, p_fragmentShader });
}

Wrapper::Program::~Program()
{
    if (m_program) {
        SPDLOG_INFO("Destroy OpenGL Program - {}", m_program);
        glDeleteProgram(m_program);
    }
}

void Wrapper::Program::Use(void)
{
    glUseProgram(m_program);
}

void Wrapper::Program::SetUniform(const std::string& name, int value) const
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform1i(loc, value);
}

void Wrapper::Program::SetUniform(const std::string& name, float value) const
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform1f(loc, value);
}

void Wrapper::Program::SetUniform(const std::string& name, const glm::vec2& value) const
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform2fv(loc, 1, glm::value_ptr(value));
}

void Wrapper::Program::SetUniform(const std::string& name, const glm::vec3& value) const
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Wrapper::Program::SetUniform(const std::string& name, const glm::vec4& value) const
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void Wrapper::Program::SetUniform(const std::string& name, const glm::mat4& value) const
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Wrapper::Program::LinkShaders(const std::vector<Shader*>& shaders)
{
    m_program = glCreateProgram();

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
        delete shader;
    }

    SPDLOG_INFO("Create OpenGL Program - {}", m_program);
}
