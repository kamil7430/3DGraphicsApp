#ifndef INC_3DGRAPHICSAPP_PINE_TREE_H
#define INC_3DGRAPHICSAPP_PINE_TREE_H

#include "../object_base.h"

static const char pineTree[] = {
    #embed "../../../assets/pine-tree.glb"
    , 0
};

class PineTree final : public ObjectBase {
public:
    PineTree() {
        std::vector<float> vertices = {};
        std::vector<unsigned int> indices = {};
        std::vector<glm::vec4> materialColors = {};
        std::vector<SubMesh> subMeshes = {};

        initialize3dModel(vertices, indices, pineTree, sizeof(pineTree), "glb", materialColors, subMeshes, 3.0f);

        openGlModel = new ObjectOpenGlModel(vertices, indices, subMeshes, materialColors);
    }
};

#endif //INC_3DGRAPHICSAPP_PINE_TREE_H