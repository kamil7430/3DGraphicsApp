#ifndef INC_3DGRAPHICSAPP_SPORTS_CAR_H
#define INC_3DGRAPHICSAPP_SPORTS_CAR_H

#include "../object_base.h"

static const char sportsCar[] = {
    #embed "../../../assets/sports-car.glb"
    , 0
};

class SportsCar final : public ObjectBase {
public:
    SportsCar() {
        std::vector<float> vertices = {};
        std::vector<unsigned int> indices = {};
        std::vector<glm::vec4> materialColors = {};
        std::vector<SubMesh> subMeshes = {};

        initialize3dModel(vertices, indices, sportsCar, sizeof(sportsCar), "glb", materialColors, subMeshes, 1.0f);

        openGlModel = new ObjectOpenGlModel(vertices, indices, subMeshes, materialColors);
    }
};

#endif //INC_3DGRAPHICSAPP_SPORTS_CAR_H