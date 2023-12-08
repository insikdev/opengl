#pragma once

namespace Wrapper {
class Window;
class Program;
class Buffer;
class VertexLayout;
class Texture;

class Context {
public:
    Context(const Window*);
    ~Context();

public:
    void HandleInput();
    void HandleMouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);
    void Render(void);
    void Update(float dt);

private:
    void CreateContext(void);
    void CreateProgram(void);
    void InitModel(void);
    void LoadImages(void);

private:
    const Window* p_window;
    Program* p_program;
    VertexLayout* m_vertexLayout;
    Buffer* m_vertexBuffer;
    Buffer* m_indexBuffer;
    Texture* p_texture1;
    Texture* p_texture2;
    glm::vec4 m_clearColor { glm::vec4(0.1f, 0.2f, 0.3f, 0.0f) };

private:
    ImGuiContext* m_ImGuiContext;

private: // camera
    glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };
    float m_cameraPitch { 0.0f };
    float m_cameraYaw { 0.0f };
    bool m_cameraControl { false };
    glm::vec2 m_prevMousePos { glm::vec2(0.0f) };
};
}
