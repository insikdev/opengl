#include "pch.h"
#include "gl_vertex_layout.h"

Wrapper::VertexLayout::VertexLayout()
{
    CreateLayout();
}

Wrapper::VertexLayout::~VertexLayout()
{
    if (m_vertexArrayObject) {
        SPDLOG_INFO("Destory Vertex Layout Object - {}", m_vertexArrayObject);
        glDeleteVertexArrays(1, &m_vertexArrayObject);
    }
}

void Wrapper::VertexLayout::SetAttribute(uint32_t index, int count, uint32_t type, bool normalized, GLsizei stride, uint64_t offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, count, type, normalized, stride, (const void*)offset);
}

void Wrapper::VertexLayout::CreateLayout()
{
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    SPDLOG_INFO("Create Vertex Layout Object - {}", m_vertexArrayObject);
}
