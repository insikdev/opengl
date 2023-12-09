#include "pch.h"
#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(const std::string& path)
{
    LoadFromFile(path);
}

Image::Image(int width, int height, int channelCount)
    : m_width { width }
    , m_height { height }
    , m_channelCount { channelCount }
{
    m_data = (uint8_t*)malloc(m_width * m_height * m_channelCount);
}

Image::~Image()
{
    if (m_data) {
        stbi_image_free(m_data);
        SPDLOG_INFO("Delete Image");
    }
}

Image* Image::CreateSingleColorImage(int width, int height, const glm::vec4& color)
{
    glm::vec4 clamped = glm::clamp(color * 255.0f, 0.0f, 255.0f);
    uint8_t rgba[4] = { (uint8_t)clamped.r, (uint8_t)clamped.g, (uint8_t)clamped.b, (uint8_t)clamped.a };

    Image* image = new Image { width, height, 4 };

    for (int i = 0; i < width * height; i++) {
        memcpy(image->m_data + 4 * i, rgba, 4);
    }

    return image;
}

void Image::LoadFromFile(const std::string& path)
{
    stbi_set_flip_vertically_on_load(true);
    m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_channelCount, 0);

    if (!m_data) {
        SPDLOG_ERROR("failed to load image : {}", path);
        throw std::runtime_error("Failed to load image : " + path);
    }

    SPDLOG_INFO("Load Image : {} ({} x {})", path, m_width, m_height);
}
