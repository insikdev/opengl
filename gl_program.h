#pragma once

namespace Wrapper {
class Shader;

class Program {
public:
    Program(const std::vector<Shader*>&);
    ~Program();

public:
    void Use() { glUseProgram(m_program); }

public: // getter
    uint32_t Get() { return m_program; }

private:
    void LinkShaders(const std::vector<Shader*>&);

private:
    uint32_t m_program {};
};
}
