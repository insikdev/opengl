#include "pch.h"
#include "gl_buffer.h"

Wrapper::Buffer::Buffer(uint32_t type, uint32_t usage, const void* data, size_t dataSize)
    : m_type { type }
    , m_usage { usage }
{
    CreateBuffer(data, dataSize);
}

Wrapper::Buffer::~Buffer()
{
    if (m_buffer) {
        glDeleteBuffers(1, &m_buffer);

        SPDLOG_INFO("Destroy Buffer");
    }
}

void Wrapper::Buffer::CreateBuffer(const void* data, size_t dataSize)
{
    SPDLOG_INFO("Create Buffer");

    glGenBuffers(1, &m_buffer);
    glBindBuffer(m_type, m_buffer);
    glBufferData(m_type, dataSize, data, m_usage);
}
