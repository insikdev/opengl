#include "pch.h"
#include "gl_context.h"
#include "gl_window.h"
#include "gl_program.h"
#include "image.h"
#include "gl_texture.h"
#include "utils.h"
#include "mesh.h"
#include "model.h"

Wrapper::Context::Context(const Window* pWindow)
    : p_window { pWindow }
{
    CreateContext();
    CreatePrograms();
    CreateModels();

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

    delete m_model;
    delete m_box;
    delete p_simpleProgram;
    delete p_program;

    SPDLOG_INFO("Destroy OpenGL Context");
}

void Wrapper::Context::HandleInput()
{
    if (!m_cameraControl) {
        return;
    }

    const float cameraSpeed = 0.005f;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear frame buffer

    m_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f))
        * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    auto view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    auto projection = glm::perspective(glm::radians(45.0f), p_window->GetAspectRatio(), 0.01f, 50.0f);

    glm::vec3 lightPos = m_light.position;
    glm::vec3 lightDir = m_light.direction;

    if (m_flashLightMode) {
        lightPos = m_cameraPos;
        lightDir = m_cameraFront;
    } else {
        // simple program for light
        auto lightModelTransform = glm::translate(glm::mat4(1.0), m_light.position) * glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
        p_simpleProgram->Use();
        p_simpleProgram->SetUniform("color", glm::vec4(m_light.ambient + m_light.diffuse, 1.0f));
        p_simpleProgram->SetUniform("transform", projection * view * lightModelTransform);
        m_box->Draw(p_simpleProgram);
    }

    // model
    auto world = glm::mat4(1.0f);
    p_program->Use();
    p_program->SetUniform("world", world);
    p_program->SetUniform("view", view);
    p_program->SetUniform("projection", projection);
    p_program->SetUniform("viewPos", m_cameraPos);
    p_program->SetUniform("light.position", lightPos);
    p_program->SetUniform("light.direction", lightDir);
    p_program->SetUniform("light.cutoff", glm::vec2(cosf(glm::radians(m_light.cutoff[0])), cosf(glm::radians(m_light.cutoff[0] + m_light.cutoff[1]))));
    p_program->SetUniform("light.attenuation", Utils::GetAttenuationCoeff(m_light.distance));
    p_program->SetUniform("light.ambient", m_light.ambient);
    p_program->SetUniform("light.diffuse", m_light.diffuse);
    p_program->SetUniform("light.specular", m_light.specular);

    m_model->Draw(p_program);

    // ImGui
    if (ImGui::Begin("ui window")) {
        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Checkbox("animation", &m_animation);
        if (ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor))) {
            glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
        }
        if (ImGui::CollapsingHeader("camera")) {
            ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
            ImGui::DragFloat("camera yaw", &m_cameraYaw, 0.5f);
            ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
            if (ImGui::Button("reset camera")) {
                m_cameraYaw = 0.0f;
                m_cameraPitch = 0.0f;
                m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
            }
        }

        if (ImGui::CollapsingHeader("light")) {
            ImGui::DragFloat3("l.position", glm::value_ptr(m_light.position), 0.01f);
            ImGui::DragFloat3("l.direction", glm::value_ptr(m_light.direction), 0.01f);
            ImGui::ColorEdit3("l.ambient", glm::value_ptr(m_light.ambient));
            ImGui::ColorEdit3("l.diffuse", glm::value_ptr(m_light.diffuse));
            ImGui::ColorEdit3("l.specular", glm::value_ptr(m_light.specular));
            ImGui::Checkbox("l.flash", &m_flashLightMode);
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
    SPDLOG_INFO("Create OpenGL Context - version : {}", (const char*)glVersion);

    m_ImGuiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(m_ImGuiContext);

    ImGui_ImplGlfw_InitForOpenGL(p_window->GetGLFWwindow(), false);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();
}

void Wrapper::Context::CreatePrograms(void)
{
    p_simpleProgram = new Wrapper::Program { "shaders/simple.vert", "shaders/simple.frag" };
    p_program = new Wrapper::Program { "shaders/light.vert", "shaders/light.frag" };
}

void Wrapper::Context::CreateModels(void)
{
    m_box = Mesh::CreateCube();
    m_model = new Model { "model/backpack.obj" };
}
