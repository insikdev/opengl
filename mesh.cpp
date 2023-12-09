#include "pch.h"
#include "mesh.h"
#include "gl_buffer.h"
#include "gl_vertex_layout.h"
#include "geometry.h"
#include "material.h"
#include "gl_program.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType)
    : m_primitiveType { primitiveType }
{
    m_vertexLayout = new Wrapper::VertexLayout {};

    m_vertexBuffer = new Wrapper::Buffer { GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(Vertex), vertices.size() };
    m_indexBuffer = new Wrapper::Buffer { GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t), indices.size() };

    m_vertexLayout->SetAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    m_vertexLayout->SetAttribute(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
    m_vertexLayout->SetAttribute(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));
}

Mesh::~Mesh()
{
    delete m_vertexBuffer;
    delete m_indexBuffer;
    delete m_vertexLayout;
}

void Mesh::Draw(const Wrapper::Program* program)
{
    m_vertexLayout->Bind();
    if (m_material) {
        m_material->SetToProgram(program);
    }
    glDrawElements(m_primitiveType, static_cast<GLsizei>(m_indexBuffer->GetCount()), GL_UNSIGNED_INT, 0);
}

Mesh* Mesh::CreateCube()
{
    auto geometry = Geometry::CreateCube();
    return new Mesh { geometry.vertices, geometry.indices };
}
