#pragma once

#include "pch.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Wrapper {
class Program;
}
class Mesh;
class Material;

class Model {
public:
    Model(const std::string& filename);
    ~Model();

public:
    void Draw(const Wrapper::Program* program) const;

public: // getter
    int GetMeshCount() const { return (int)m_meshes.size(); }
    Mesh* GetMesh(int index) const { return m_meshes[index]; }

private:
    void LoadByAssimp(const std::string& filename);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void ProcessNode(aiNode* node, const aiScene* scene);

private:
    std::vector<Mesh*> m_meshes;
    std::vector<Material*> m_materials;
};
