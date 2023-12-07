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

void App::Run(void)
{
    while (!p_window->ShouldClose()) {
        glfwPollEvents();
        p_context->Update(1.0f);
        p_context->Render();
    }
}

void App::Init(void)
{
    p_window = new Wrapper::Window { "OpenGL Tutorial" };
    p_context = new Wrapper::Context { p_window };

    OnFramebufferSizeChange(p_window->GetGLFWwindow(), p_window->GetWidth(), p_window->GetHeight());
    glfwSetFramebufferSizeCallback(p_window->GetGLFWwindow(), OnFramebufferSizeChange);
    glfwSetKeyCallback(p_window->GetGLFWwindow(), OnKeyEvent);
}
