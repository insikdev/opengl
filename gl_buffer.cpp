#include "pch.h"
#include "gl_buffer.h"

Wrapper::Buffer::Buffer(uint32_t type, uint32_t usage, const void* data, size_t stride, size_t count)
    : m_type { type }
    , m_usage { usage }
    , m_stride { stride }
    , m_count { count }
{
    CreateBuffer(data);
}

Wrapper::Buffer::~Buffer()
{
    if (m_buffer) {
        SPDLOG_INFO("Destroy OpenGL Buffer - {}", m_buffer);
        glDeleteBuffers(1, &m_buffer);
    }
}

void Wrapper::Buffer::Bind(void)
{
    glBindBuffer(m_type, m_buffer);
}

void Wrapper::Buffer::CreateBuffer(const void* data)
{
    glGenBuffers(1, &m_buffer);
    Bind();
    glBufferData(m_type, m_stride * m_count, data, m_usage);

    SPDLOG_INFO("Create OpenGL Buffer - {}", m_buffer);
}
