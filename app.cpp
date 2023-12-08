#include "pch.h"
#include "app.h"
#include "gl_window.h"
#include "gl_context.h"

App::App()
{
    Init();
}

App::~App()
{
    delete p_context;
    delete p_window;
}

void App::OnFramebufferSizeChange(GLFWwindow* window, int width, int height)
{
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    glViewport(0, 0, width, height);
    reinterpret_cast<App*>(glfwGetWindowUserPointer(window))->p_window->SetWidthHeight(width, height);
}

void App::OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
        key, scancode,
        action == GLFW_PRESS ? "Pressed" : action == GLFW_RELEASE ? "Released"
            : action == GLFW_REPEAT                               ? "Repeat"
                                                                  : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");

    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void App::OnMouseButton(GLFWwindow* window, int button, int action, int modifier)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    reinterpret_cast<App*>(glfwGetWindowUserPointer(window))->p_context->MouseButton(button, action, x, y);
}

void App::OnCursorPos(GLFWwindow* window, double x, double y)
{
    reinterpret_cast<App*>(glfwGetWindowUserPointer(window))->p_context->HandleMouseMove(x, y);
}

inline void App::OnCharEvent(GLFWwindow* window, unsigned int ch)
{
    ImGui_ImplGlfw_CharCallback(window, ch);
}

inline void App::OnScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void App::Run(void)
{
    while (!p_window->ShouldClose()) {
        glfwPollEvents();

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        p_context->HandleInput();
        p_context->Update(1.0f);
        p_context->Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(p_window->GetGLFWwindow()); // double buffering
    }
}

void App::Init(void)
{
    p_window = new Wrapper::Window { "OpenGL Tutorial" };
    p_context = new Wrapper::Context { p_window };

    glfwSetWindowUserPointer(p_window->GetGLFWwindow(), this);

    // set callback
    glfwSetFramebufferSizeCallback(p_window->GetGLFWwindow(), OnFramebufferSizeChange);
    glfwSetKeyCallback(p_window->GetGLFWwindow(), OnKeyEvent);
    glfwSetCursorPosCallback(p_window->GetGLFWwindow(), OnCursorPos);
    glfwSetMouseButtonCallback(p_window->GetGLFWwindow(), OnMouseButton);
    glfwSetCharCallback(p_window->GetGLFWwindow(), OnCharEvent);
    glfwSetScrollCallback(p_window->GetGLFWwindow(), OnScroll);

    OnFramebufferSizeChange(p_window->GetGLFWwindow(), p_window->GetWidth(), p_window->GetHeight());
}
