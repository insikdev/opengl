#pragma once
namespace Wrapper {
class VertexLayout {
public:
    VertexLayout();
    ~VertexLayout();

public:
    void SetAttribute(uint32_t index, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset);

private:
    void CreateLayout();

private:
    uint32_t m_vertexArrayObject {};
};
}
