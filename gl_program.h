#pragma once

namespace Wrapper {
class Shader;

class Program {
public:
    Program(const std::vector<Shader*>&);
    ~Program();

public:
    void Use() { glUseProgram(m_program); }
    void SetUniform(const std::string& name, int value) const;
    void SetUniform(const std::string& name, const glm::mat4& value) const;

public: // getter
    uint32_t Get() { return m_program; }

private:
    void LinkShaders(const std::vector<Shader*>&);

private:
    uint32_t m_program {};
};
}
