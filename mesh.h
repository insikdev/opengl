#pragma once

class Material;

namespace Wrapper {
class VertexLayout;
class Buffer;
class Program;
}

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType = GL_TRIANGLES);
    ~Mesh();

public:
    void Draw(const Wrapper::Program*);
    static Mesh* CreateCube(void);

public:
    Material* GetMaterial() const { return m_material; }
    void SetMaterial(Material* material) { m_material = material; }

private:
    uint32_t m_primitiveType { GL_TRIANGLES };
    Wrapper::VertexLayout* m_vertexLayout {};
    Wrapper::Buffer* m_vertexBuffer {};
    Wrapper::Buffer* m_indexBuffer {};
    Material* m_material {};
};
