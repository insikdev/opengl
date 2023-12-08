#pragma once

namespace Wrapper {
class Window {
public:
    Window(const char*);
    ~Window();

public:
    bool ShouldClose(void);
    void SetWidthHeight(uint32_t width, uint32_t height)
    {
        m_width = width;
        m_height = height;
    }

public: // getter
    GLFWwindow* GetGLFWwindow() const { return m_window; }
    uint32_t GetWidth() const { return m_width; }
    uint32_t GetHeight() const { return m_height; }
    float GetAspectRatio() const { return m_width / static_cast<float>(m_height); }

private:
    void CreateGLFW(void);

private:
    GLFWwindow* m_window;
    const char* m_title;
    uint32_t m_width { 960 };
    uint32_t m_height { 960 };
};
}
