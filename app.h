#pragma once

namespace Wrapper {
class Window;
class Context;
}

class App {
public:
    App();
    ~App();

public:
    void Run(void);

public: // callback
    static void OnFramebufferSizeChange(GLFWwindow* window, int width, int height);
    static void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void OnCursorPos(GLFWwindow* window, double x, double y);
    static void OnMouseButton(GLFWwindow* window, int button, int action, int modifier);
    static void OnCharEvent(GLFWwindow* window, unsigned int ch);
    static void OnScroll(GLFWwindow* window, double xoffset, double yoffset);

private:
    void Init(void);

private:
    Wrapper::Window* p_window;
    Wrapper::Context* p_context;
};
