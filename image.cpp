#include "pch.h"
#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(const std::string& path)
{
    LoadFromFile(path);
}

Image::~Image()
{
    if (m_data) {
        stbi_image_free(m_data);
    }
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
