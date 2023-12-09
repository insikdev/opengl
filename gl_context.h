#pragma once

class Mesh;
class Model;

namespace Wrapper {
class Window;
class Program;

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
    void CreatePrograms(void);
    void CreateModels(void);

private:
    const Window* p_window;
    Program* p_program;
    Program* p_simpleProgram;
    glm::vec4 m_clearColor { glm::vec4(0.1f, 0.2f, 0.3f, 0.0f) };

private:
    Mesh* m_box;
    Model* m_model;

private:
    ImGuiContext* m_ImGuiContext;
    bool m_animation { true };
    bool m_flashLightMode { true };

private: // camera
    glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };
    float m_cameraPitch { 0.0f };
    float m_cameraYaw { 0.0f };
    bool m_cameraControl { false };
    glm::vec2 m_prevMousePos { glm::vec2(0.0f) };

private:
    struct Light {
        glm::vec3 position { glm::vec3(2.0f, 2.0f, 2.0f) };
        glm::vec3 direction { glm::vec3(-1.0f, -1.0f, -1.0f) };
        glm::vec2 cutoff { glm::vec2(20.0f, 5.0f) };
        float distance { 32.0f };
        glm::vec3 ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
        glm::vec3 diffuse { glm::vec3(0.8f, 0.8f, 0.8f) };
        glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light m_light;
};
}
