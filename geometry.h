#pragma once

#include "pch.h"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

class Geometry {
public:
    static MeshData CreateCube(void)
    {
        MeshData out;
        out.vertices = {
            // Front face
            { { -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
            { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
            { { 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
            { { -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },

            // Back face
            { { 1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
            { { -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
            { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },
            { { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } },

            // Top face
            { { -1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
            { { 1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
            { { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
            { { -1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },

            // Bottom face
            { { -1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
            { { 1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
            { { 1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } },
            { { -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } },

            // Right face
            { { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
            { { 1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
            { { 1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
            { { 1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },

            // Left face
            { { -1.0f, 1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
            { { -1.0f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
            { { -1.0f, -1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
            { { -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } }
        };

        // VK_FRONT_FACE_COUNTER_CLOCKWISE
        out.indices = {
            0, 2, 3, 0, 1, 2, // Front face
            4, 6, 7, 4, 5, 6, // Back face
            8, 10, 11, 8, 9, 10, // Top face
            12, 14, 15, 12, 13, 14, // Bottom face
            16, 18, 19, 16, 17, 18, // Right face
            20, 22, 23, 20, 21, 22 // Left face
        };

        return out;
    }
};
