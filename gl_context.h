#pragma once

namespace Wrapper {
class Window;
class Program;
class Buffer;
class VertexLayout;

class Context {
public:
    Context(const Window*);
    ~Context();

public:
    void Render(void);
    void Update(float dt);

private:
    void CreateContext(void);
    void CreateProgram(void);
    void InitModel(void);

private:
    const Window* p_window;
    Program* p_program;
    VertexLayout* m_vertexLayout;
    Buffer* m_vertexBuffer;
    Buffer* m_indexBuffer;
};
}
