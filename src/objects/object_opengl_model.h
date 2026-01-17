#ifndef INC_3DGRAPHICSAPP_OBJECT_OPENGL_MODEL_H
#define INC_3DGRAPHICSAPP_OBJECT_OPENGL_MODEL_H

#include <vector>
#include <glm/glm.hpp>

#include "model_initializer.h"
#include "../shader.h"

class ObjectOpenGlModel {
private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    int indicesCount;
    Shader shader;
    std::vector<SubMesh> subMeshes;
    std::vector<glm::vec4> materialColors;

public:
    ObjectOpenGlModel(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, const char vertexShader[],
        const char fragmentShader[], const std::vector<SubMesh> &subMeshes, const std::vector<glm::vec4> &materialColors);
    void draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection);
    ~ObjectOpenGlModel();
};

#endif //INC_3DGRAPHICSAPP_OBJECT_OPENGL_MODEL_H