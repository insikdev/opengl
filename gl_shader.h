#pragma once

namespace Wrapper {
class Shader {
public:
    Shader(const std::string& filename, GLenum shaderType);
    ~Shader();

public: // getter
    uint32_t GetShader() { return m_shader; }

private:
    void CreateShader(const std::string&, GLenum);

private:
    uint32_t m_shader {};
};
}
