#ifndef INC_3DGRAPHICSAPP_RUBBER_DUCKY_H
#define INC_3DGRAPHICSAPP_RUBBER_DUCKY_H

#include "../object_base.h"

static const char rubberDucky[] = {
    #embed "../../../assets/rubber-ducky.3ds"
    , 0
};

class RubberDucky final : public ObjectBase {
public:
    RubberDucky() {
        std::vector<float> vertices = {};
        std::vector<unsigned int> indices = {};
        std::vector<glm::vec4> materialColors = {};
        std::vector<SubMesh> subMeshes = {};

        initialize3dModel(vertices, indices, rubberDucky, sizeof(rubberDucky), "3ds", materialColors, subMeshes, 3.0f);

        openGlModel = new ObjectOpenGlModel(vertices, indices, subMeshes, materialColors);
    }
};

#endif //INC_3DGRAPHICSAPP_RUBBER_DUCKY_H