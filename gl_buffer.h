#pragma once

namespace Wrapper {
class Buffer {
public:
    Buffer(uint32_t type, uint32_t usage, const void* data, size_t dataSize);
    ~Buffer();

public: // getter
    uint32_t GetBuffer() { return m_buffer; }

private:
    void CreateBuffer(const void* data, size_t dataSize);

private:
    uint32_t m_buffer {};
    uint32_t m_type {};
    uint32_t m_usage {};
};
}
