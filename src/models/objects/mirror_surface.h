#ifndef INC_3DGRAPHICSAPP_MIRROR_SURFACE_H
#define INC_3DGRAPHICSAPP_MIRROR_SURFACE_H

#include "../object_base.h"

class MirrorSurface final : public ObjectBase {
public:
    MirrorSurface() {
        std::vector<float> vertices = {
            // Vertices            // Normals
            -1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            1, 2, 3,
        };

        std::vector<SubMesh> subMeshes = {};
        subMeshes.push_back(SubMesh{6, 0, 0});

        std::vector<glm::vec4> materials = {};
        materials.push_back(glm::vec4(0.8f, 0.9f, 1.0f, 0.2f));

        openGlModel = new ObjectOpenGlModel(vertices, indices, subMeshes, materials);
    }
};

#endif //INC_3DGRAPHICSAPP_MIRROR_SURFACE_H