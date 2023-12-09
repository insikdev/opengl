#include "pch.h"
#include "model.h"
#include "mesh.h"
#include "image.h"
#include "gl_texture.h"
#include "material.h"

Model::Model(const std::string& filename)
{
    LoadByAssimp(filename);
}

Model::~Model()
{
    for (auto& mesh : m_meshes) {
        delete mesh;
    }

    for (auto& material : m_materials) {
        delete material;
    }
}

void Model::Draw(const Wrapper::Program* program) const
{
    for (auto& mesh : m_meshes) {
        mesh->Draw(program);
    }
}

void Model::LoadByAssimp(const std::string& filename)
{
    SPDLOG_INFO("Load Model : {}", filename);

    Assimp::Importer importer;
    auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        SPDLOG_ERROR("failed to load model : {}", filename);
        throw std::runtime_error("Failed to load model : " + filename);
    }

    auto dirname = filename.substr(0, filename.find_last_of("/"));

    auto LoadTexture = [&](aiMaterial* material, aiTextureType type) -> std::shared_ptr<Wrapper::Texture> {
        if (material->GetTextureCount(type) <= 0) {
            return nullptr;
        }

        aiString filepath;
        material->GetTexture(type, 0, &filepath);

        Image* image = new Image { fmt::format("{}/{}", dirname, filepath.C_Str()) };
        return std::make_shared<Wrapper::Texture>(image);
    };

    for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
        auto material = scene->mMaterials[i];
        auto glMaterial = new Material {};
        glMaterial->diffuse = LoadTexture(material, aiTextureType_DIFFUSE);
        glMaterial->specular = LoadTexture(material, aiTextureType_SPECULAR);
        m_materials.push_back(glMaterial);
    }

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    SPDLOG_INFO("process mesh: {}, #vert: {}, #face: {}", mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);

    std::vector<Vertex> vertices;
    vertices.resize(mesh->mNumVertices);

    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        auto& v = vertices[i];
        v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        v.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        v.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }

    std::vector<uint32_t> indices;
    indices.resize(mesh->mNumFaces * 3);

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        indices[3 * i] = mesh->mFaces[i].mIndices[0];
        indices[3 * i + 1] = mesh->mFaces[i].mIndices[1];
        indices[3 * i + 2] = mesh->mFaces[i].mIndices[2];
    }

    Mesh* glMesh = new Mesh { vertices, indices };
    if (mesh->mMaterialIndex >= 0) {
        glMesh->SetMaterial(m_materials[mesh->mMaterialIndex]);
    }
    m_meshes.push_back(glMesh);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        auto meshIndex = node->mMeshes[i];
        auto mesh = scene->mMeshes[meshIndex];
        ProcessMesh(mesh, scene);
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}
