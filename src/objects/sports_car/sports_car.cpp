#include "sports_car.h"

#include <glad/gl.h>
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

SportsCar::SportsCar() : shader(vertexShader, fragmentShader) {
    std::vector<float> vertices = {};
    std::vector<unsigned int> indices = {};

    initialize3dModel(vertices, indices, sportsCar, sizeof(sportsCar), "glb", materialColors, subMeshes);

    indicesCount = indices.size();

    shader.use();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void SportsCar::draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection) {
    shader.use();

    glUniformMatrix4fv(shader.getUniformLocation("uView"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.getUniformLocation("uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(shader.getUniformLocation("uModel"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    for (const auto &[indexCount, baseIndex, materialIndex] : subMeshes) {
        glm::vec4 colorVec = glm::vec4(1.0, 0.0, 1.0, 1.0);

        if (materialIndex < materialColors.size()) {
            colorVec = materialColors[materialIndex];
        }

        glUniform4fv(shader.getUniformLocation("uColor"), 1, glm::value_ptr(colorVec));

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(baseIndex));
    }
}

SportsCar::~SportsCar() {
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
