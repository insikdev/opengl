#include "pch.h"
#include "gl_context.h"
#include "gl_window.h"
#include "gl_shader.h"
#include "gl_program.h"
#include "gl_buffer.h"
#include "gl_vertex_layout.h"

Wrapper::Context::Context(const Window* pWindow)
    : p_window { pWindow }
{
    CreateContext();
    CreateProgram();
    InitModel();
}

Wrapper::Context::~Context()
{
    delete p_program;
    delete m_vertexLayout;
    delete m_vertexBuffer;
    delete m_indexBuffer;
}

void Wrapper::Context::Render(void)
{
    glClear(GL_COLOR_BUFFER_BIT); // clear frame buffer

    p_program->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(p_window->GetGLFWwindow()); // double buffering
}

void Wrapper::Context::Update(float dt)
{
}

void Wrapper::Context::CreateContext(void)
{
    SPDLOG_INFO("Create OpenGL Context");

    glfwMakeContextCurrent(p_window->GetGLFWwindow());

    // glad : OpenGL Loader
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to init GLAD");
    }

    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}", (const char*)glVersion);
}

void Wrapper::Context::CreateProgram(void)
{
    auto p_vertexShader = new Wrapper::Shader { "shaders/simple.vert", GL_VERTEX_SHADER };
    auto p_fragmentShader = new Wrapper::Shader { "shaders/simple.frag", GL_FRAGMENT_SHADER };

    p_program = new Wrapper::Program { { p_vertexShader, p_fragmentShader } };

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // set clear color

    delete p_vertexShader;
    delete p_fragmentShader;
}

void Wrapper::Context::InitModel(void)
{
    float vertices[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, // top left
    };
    uint32_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    m_vertexLayout = new VertexLayout {};
    m_vertexBuffer = new Buffer { GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 12 };
    m_vertexLayout->SetAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    m_indexBuffer = new Buffer { GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6 };
}
