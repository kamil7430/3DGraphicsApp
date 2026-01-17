#ifndef INC_3DGRAPHICSAPP_SPHERE_H
#define INC_3DGRAPHICSAPP_SPHERE_H

#include <cmath>
#include <vector>

#include "../object_base.h"

class Sphere final : public ObjectBase {
public:
    Sphere(const int stackCount, const int sectorCount) {
        // Calculate vertices array
        // Since r = 1, normal vectors are equal to vertices coords
        std::vector<float> vertices = {};

        const float sectorStep = 2 * M_PI / sectorCount;
        const float stackStep = M_PI / stackCount;

        for (int i = 0; i <= stackCount; ++i) {
            const float stackAngle = M_PI / 2 - i * stackStep;
            const float xy = cosf(stackAngle);
            float z = sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j) {
                const float sectorAngle = j * sectorStep;

                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }

        // Calculate indices array
        std::vector<unsigned int> indices = {};
        for (int i = 0; i < stackCount; ++i) {
            int k1 = i * (sectorCount + 1); // beginning of current stack
            int k2 = k1 + sectorCount + 1; // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != stackCount - 1) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        std::vector<SubMesh> subMeshes{};
        subMeshes.push_back(SubMesh{static_cast<unsigned int>(indices.size()) * 3, 0, 0});
        std::vector<glm::vec4> materials{};
        materials.push_back(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

        openGlModel = new ObjectOpenGlModel(vertices, indices, subMeshes, materials);
    }
};

#endif //INC_3DGRAPHICSAPP_SPHERE_H