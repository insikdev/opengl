#pragma once

namespace Wrapper {
class VertexLayout {
public:
    VertexLayout();
    ~VertexLayout();

public:
    void Bind(void);
    void SetAttribute(uint32_t index, int count, uint32_t type, bool normalized, GLsizei stride, uint64_t offset);

private:
    void CreateLayout();

private:
    uint32_t m_vertexArrayObject {};
};
}
