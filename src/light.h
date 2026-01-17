#ifndef INC_3DGRAPHICSAPP_LIGHT_H
#define INC_3DGRAPHICSAPP_LIGHT_H

#include <glm/vec3.hpp>

struct LightSource {
    glm::vec3 position;
    glm::vec3 color;
    int reflection;
    glm::vec3 direction;
};

#endif //INC_3DGRAPHICSAPP_LIGHT_H