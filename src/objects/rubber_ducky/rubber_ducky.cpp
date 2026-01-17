#include "rubber_ducky.h"

#include <glm/gtc/type_ptr.hpp>

static const char vertexShader[] = {
    #embed "../../shaders/shader.vert"
    , 0
};

static const char fragmentShader[] = {
    #embed "../../shaders/shader.frag"
    , 0
};

static const char rubberDucky[] = {
    #embed "../../../assets/rubber-ducky.3ds"
    , 0
};

RubberDucky::RubberDucky() {
    std::vector<float> vertices = {};
    std::vector<unsigned int> indices = {};
    std::vector<glm::vec4> materialColors = {};
    std::vector<SubMesh> subMeshes = {};

    initialize3dModel(vertices, indices, rubberDucky, sizeof(rubberDucky), "3ds", materialColors, subMeshes, 3.0f);

    openGlModel = new ObjectOpenGlModel(vertices, indices, vertexShader, fragmentShader, subMeshes, materialColors);
}
