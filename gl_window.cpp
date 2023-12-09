#include "pch.h"
#include "gl_window.h"

Wrapper::Window::Window(const char* title)
    : m_title { title }
{
    CreateGLFW();
}

Wrapper::Window::~Window()
{
    SPDLOG_INFO("Destroy GLFW");
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Wrapper::Window::ShouldClose(void)
{
    return glfwWindowShouldClose(m_window);
}

void Wrapper::Window::CreateGLFW(void)
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to init GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
    assert(m_window != nullptr);

    SPDLOG_INFO("Create GLFW");
}
