#include "rubber_ducky.h"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

static const char vertexShader[] = {
    #embed "rubber_ducky.vert"
    , 0
};

static const char fragmentShader[] = {
    #embed "rubber_ducky.frag"
    , 0
};

static const char rubberDucky[] = {
    #embed "../../../assets/rubber-ducky.3ds"
    , 0
};

RubberDucky::RubberDucky() : shader(vertexShader, fragmentShader) {
    std::vector<float> vertices = {};
    std::vector<unsigned int> indices = {};

    initialize3dModel(vertices, indices, rubberDucky, sizeof(rubberDucky), "3ds", materialColors, subMeshes, 1.0f);

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

void RubberDucky::draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection) {
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

RubberDucky::~RubberDucky() {
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
