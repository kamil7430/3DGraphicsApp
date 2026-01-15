#ifndef INC_3DGRAPHICSAPP_SPHERE_H
#define INC_3DGRAPHICSAPP_SPHERE_H

#include <glm/glm.hpp>

#include "../../shader.h"

class Sphere {
private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    int indicesCount;
    Shader shader;

public:
    Sphere(const int stackCount, const int sectorCount);
    void draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection);
    ~Sphere();
};

#endif //INC_3DGRAPHICSAPP_SPHERE_H