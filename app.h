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
    static void OnFramebufferSizeChange(GLFWwindow* window, int width, int height)
    {
        SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
        glViewport(0, 0, width, height);
    }

    static void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
            key, scancode,
            action == GLFW_PRESS ? "Pressed" : action == GLFW_RELEASE ? "Released"
                : action == GLFW_REPEAT                               ? "Repeat"
                                                                      : "Unknown",
            mods & GLFW_MOD_CONTROL ? "C" : "-",
            mods & GLFW_MOD_SHIFT ? "S" : "-",
            mods & GLFW_MOD_ALT ? "A" : "-");
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

public:
    void Run(void);

private:
    void Init(void);

private:
    Wrapper::Window* p_window;
    Wrapper::Context* p_context;
};
