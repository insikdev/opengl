#pragma once

namespace Wrapper {
class Texture;
class Program;
}

class Material {
public:
    Material() = default;
    ~Material() = default;

public:
    void SetToProgram(const Wrapper::Program* program) const;

public:
    std::shared_ptr<Wrapper::Texture> diffuse;
    std::shared_ptr<Wrapper::Texture> specular;
    float shininess { 32.0f };
};
