#include "pch.h"
#include "gl_texture.h"
#include "image.h"

Wrapper::Texture::Texture(const Image* pImage)
{
    CreateTexture(pImage);
}

Wrapper::Texture::~Texture()
{
    if (m_texture) {
        SPDLOG_INFO("Destroy OpenGL Texture - {}", m_texture);
        glDeleteTextures(1, &m_texture);
    }
}

void Wrapper::Texture::Bind(void) const
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Wrapper::Texture::SetFilter(uint32_t minFilter, uint32_t magFilter) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Wrapper::Texture::SetWrap(uint32_t sWrap, uint32_t tWrap) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
}

void Wrapper::Texture::CreateTexture(const Image* pImage)
{
    glGenTextures(1, &m_texture);
    Bind();

    SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    GLenum format = GL_RGBA;
    switch (pImage->GetChannelCount()) {
    default:
        break;
    case 1:
        format = GL_RED;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage->GetWidth(), pImage->GetHeight(), 0, format, GL_UNSIGNED_BYTE, pImage->GetData());
    glGenerateMipmap(GL_TEXTURE_2D);

    SPDLOG_INFO("Create OpenGL Texture - {}", m_texture);
    delete pImage;
}
