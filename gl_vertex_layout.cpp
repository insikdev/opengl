#include "pch.h"
#include "gl_vertex_layout.h"

Wrapper::VertexLayout::VertexLayout()
{
    CreateLayout();
}

Wrapper::VertexLayout::~VertexLayout()
{
    if (m_vertexArrayObject) {
        glDeleteVertexArrays(1, &m_vertexArrayObject);
        SPDLOG_INFO("Destory Vertex Layout Object");
    }
}

void Wrapper::VertexLayout::SetAttribute(uint32_t index, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, count, type, normalized, stride, (const void*)offset);
}

void Wrapper::VertexLayout::CreateLayout()
{
    SPDLOG_INFO("Create Vertex Layout Object");

    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
}
