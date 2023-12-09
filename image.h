#pragma once

class Image {
public:
    Image(const std::string&);
    Image(int width, int height, int channelCount = 4);
    ~Image();

public:
    static Image* CreateSingleColorImage(int width, int height, const glm::vec4& color);

public: // getter
    const uint8_t* GetData() const { return m_data; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetChannelCount() const { return m_channelCount; }

private:
    void LoadFromFile(const std::string&);

private:
    int m_width {};
    int m_height {};
    int m_channelCount {};
    uint8_t* m_data {};
};
