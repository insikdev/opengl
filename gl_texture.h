#pragma once

class Image;

namespace Wrapper {
class Texture {
public:
    Texture(const Image*);
    ~Texture();

public:
    void Bind(void) const;
    void SetFilter(uint32_t minFilter, uint32_t magFilter) const;
    void SetWrap(uint32_t sWrap, uint32_t tWrap) const;

private:
    void CreateTexture(const Image*);

private:
    uint32_t m_texture {};
};
}
