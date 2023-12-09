#pragma once

namespace Wrapper {
class Buffer {
public:
    Buffer(uint32_t type, uint32_t usage, const void* data, size_t stride, size_t count);
    ~Buffer();

public:
    void Bind(void);

public: // getter
    uint32_t GetBuffer() const { return m_buffer; }
    size_t GetStride() const { return m_stride; }
    size_t GetCount() const { return m_count; }

private:
    void CreateBuffer(const void* data);

private:
    uint32_t m_buffer {};
    uint32_t m_type {};
    uint32_t m_usage {};
    size_t m_stride { 0 };
    size_t m_count { 0 };
};
}
