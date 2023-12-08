#include "pch.h"
#include "gl_context.h"
#include "gl_window.h"
#include "gl_shader.h"
#include "gl_program.h"
#include "gl_buffer.h"
#include "gl_vertex_layout.h"
#include "image.h"
#include "gl_texture.h"
#include "geometry.h"

Wrapper::Context::Context(const Window* pWindow)
    : p_window { pWindow }
{
    CreateContext();
    CreateProgram();
    LoadImages();
    InitModel();

    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w); // set clear color
    glEnable(GL_DEPTH_TEST);
}

Wrapper::Context::~Context()
{
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(m_ImGuiContext);

    delete p_texture1;
    delete p_texture2;
    delete m_vertexLayout;
    delete m_vertexBuffer;
    delete m_indexBuffer;
    delete p_program;
}

void Wrapper::Context::HandleInput()
{
    if (!m_cameraControl) {
        return;
    }

    const float cameraSpeed = 0.05f;
    if (glfwGetKey(p_window->GetGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS) {
        m_cameraPos += cameraSpeed * m_cameraFront;
    }

    if (glfwGetKey(p_window->GetGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS) {
        m_cameraPos -= cameraSpeed * m_cameraFront;
    }

    auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));

    if (glfwGetKey(p_window->GetGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS) {
        m_cameraPos += cameraSpeed * cameraRight;
    }

    if (glfwGetKey(p_window->GetGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS) {
        m_cameraPos -= cameraSpeed * cameraRight;
    }

    auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));

    if (glfwGetKey(p_window->GetGLFWwindow(), GLFW_KEY_E) == GLFW_PRESS) {
        m_cameraPos += cameraSpeed * cameraUp;
    }

    if (glfwGetKey(p_window->GetGLFWwindow(), GLFW_KEY_Q) == GLFW_PRESS) {
        m_cameraPos -= cameraSpeed * cameraUp;
    }
}

void Wrapper::Context::HandleMouseMove(double x, double y)
{
    if (!m_cameraControl) {
        return;
    }

    auto pos = glm::vec2((float)x, (float)y);
    auto deltaPos = pos - m_prevMousePos;

    const float cameraRotSpeed = 0.8f;
    m_cameraYaw -= deltaPos.x * cameraRotSpeed;
    m_cameraPitch -= deltaPos.y * cameraRotSpeed;

    if (m_cameraYaw < 0.0f)
        m_cameraYaw += 360.0f;
    if (m_cameraYaw > 360.0f)
        m_cameraYaw -= 360.0f;

    if (m_cameraPitch > 89.0f)
        m_cameraPitch = 89.0f;
    if (m_cameraPitch < -89.0f)
        m_cameraPitch = -89.0f;

    m_prevMousePos = pos;
}

void Wrapper::Context::MouseButton(int button, int action, double x, double y)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            m_prevMousePos = glm::vec2((float)x, (float)y);
            m_cameraControl = true;
        } else if (action == GLFW_RELEASE) {
            m_cameraControl = false;
        }
    }
}

void Wrapper::Context::Render(void)
{
    std::vector<glm::vec3> cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear frame buffer

    p_program->Use();
    p_program->SetUniform("tex", 0);
    p_program->SetUniform("tex2", 1);

    m_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f))
        * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    auto view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    auto projection = glm::perspective(glm::radians(45.0f), p_window->GetAspectRatio(), 0.01f, 50.0f);

    for (size_t i = 0; i < cubePositions.size(); i++) {
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
        model = glm::rotate(model, glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i), glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;

        p_program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    // ImGui
    if (ImGui::Begin("ui window")) {
        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        if (ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor))) {
            glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
        }
        ImGui::Separator();
        ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
        ImGui::DragFloat("camera yaw", &m_cameraYaw, 0.5f);
        ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if (ImGui::Button("reset camera")) {
            m_cameraYaw = 0.0f;
            m_cameraPitch = 0.0f;
            m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        }
    }
    ImGui::End();
}

void Wrapper::Context::Update(float dt)
{
}

void Wrapper::Context::CreateContext(void)
{
    glfwMakeContextCurrent(p_window->GetGLFWwindow());

    // glad : OpenGL Loader
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to init GLAD");
    }

    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("Create OpenGL context - version : {}", (const char*)glVersion);

    m_ImGuiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(m_ImGuiContext);

    ImGui_ImplGlfw_InitForOpenGL(p_window->GetGLFWwindow(), false);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();
}

void Wrapper::Context::CreateProgram(void)
{
    auto p_vertexShader = new Wrapper::Shader { "shaders/simple.vert", GL_VERTEX_SHADER };
    auto p_fragmentShader = new Wrapper::Shader { "shaders/simple.frag", GL_FRAGMENT_SHADER };

    p_program = new Wrapper::Program { { p_vertexShader, p_fragmentShader } };

    delete p_vertexShader;
    delete p_fragmentShader;
}

void Wrapper::Context::InitModel(void)
{
    auto mesh = Geometry::CreateCube();

    m_vertexLayout = new VertexLayout {};
    m_vertexBuffer = new Buffer { GL_ARRAY_BUFFER, GL_STATIC_DRAW, mesh.vertices.data(), sizeof(Vertex) * mesh.vertices.size() };

    m_vertexLayout->SetAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    m_vertexLayout->SetAttribute(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color));
    m_vertexLayout->SetAttribute(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));

    m_indexBuffer = new Buffer { GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, mesh.indices.data(), sizeof(uint32_t) * mesh.indices.size() };
}

void Wrapper::Context::LoadImages(void)
{
    Image* image = new Image { "images/container.jpg" };
    p_texture1 = new Texture { image };

    Image* image2 = new Image { "images/awesomeface.png" };
    p_texture2 = new Texture { image2 };

    delete image;
    delete image2;
}
