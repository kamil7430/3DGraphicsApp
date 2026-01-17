#include "sports_car.h"

#include <glm/gtc/type_ptr.hpp>

static const char vertexShader[] = {
    #embed "sports_car.vert"
    , 0
};

static const char fragmentShader[] = {
    #embed "sports_car.frag"
    , 0
};

static const char sportsCar[] = {
    #embed "../../../assets/sports-car.glb"
    , 0
};

SportsCar::SportsCar() {
    std::vector<float> vertices = {};
    std::vector<unsigned int> indices = {};
    std::vector<glm::vec4> materialColors = {};
    std::vector<SubMesh> subMeshes = {};

    initialize3dModel(vertices, indices, sportsCar, sizeof(sportsCar), "glb", materialColors, subMeshes, 1.0f);

    openGlModel = new ObjectOpenGlModel(vertices, indices, vertexShader, fragmentShader, subMeshes, materialColors);
}
