#ifndef INC_3DGRAPHICSAPP_FLOOR_H
#define INC_3DGRAPHICSAPP_FLOOR_H

#include "object_base.h"

static constexpr char vertexShader[] = {
    #embed "../shaders/shader.vert"
    , 0
};

static constexpr char fragmentShader[] = {
    #embed "../shaders/shader.frag"
    , 0
};

class Floor final : public ObjectBase {
public:
    Floor() {
        std::vector<float> vertices = {
            // Vertices             // Normals
            -1.0f, 0.0f, -1.0f,     0.0f, 1.0f, 0.0f,
             1.0f, 0.0f, -1.0f,     0.0f, 1.0f, 0.0f,
            -1.0f, 0.0f,  1.0f,     0.0f, 1.0f, 0.0f,
             1.0f, 0.0f,  1.0f,     0.0f, 1.0f, 0.0f,
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            1, 2, 3,
        };

        std::vector<SubMesh> subMeshes = {};
        subMeshes.push_back(SubMesh{6, 0, 0});

        std::vector<glm::vec4> materials = {};
        materials.push_back(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

        openGlModel = new ObjectOpenGlModel(vertices, indices, vertexShader, fragmentShader, subMeshes, materials);
    }
};

#endif //INC_3DGRAPHICSAPP_FLOOR_H