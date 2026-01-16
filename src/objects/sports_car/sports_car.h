#ifndef INC_3DGRAPHICSAPP_SPORTS_CAR_H
#define INC_3DGRAPHICSAPP_SPORTS_CAR_H

#include <vector>
#include <glm/glm.hpp>

#include "../../model_initializer.h"
#include "../../shader.h"

class SportsCar {
private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    int indicesCount;
    Shader shader;
    std::vector<SubMesh> subMeshes;
    std::vector<glm::vec4> materialColors;

public:
    SportsCar();
    void draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection);
    ~SportsCar();
};

#endif //INC_3DGRAPHICSAPP_SPORTS_CAR_H