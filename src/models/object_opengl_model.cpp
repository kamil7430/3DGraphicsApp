#include "object_opengl_model.h"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

static constexpr char vertexShader[] = {
    #embed "../shaders/shader.vert"
    , 0
};

static constexpr char fragmentShader[] = {
    #embed "../shaders/shader.frag"
    , 0
};

ObjectOpenGlModel::ObjectOpenGlModel(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
    const std::vector<SubMesh> &subMeshes, const std::vector<glm::vec4> &materialColors) : shader(vertexShader, fragmentShader) {
    this->subMeshes = subMeshes;
    this->materialColors = materialColors;
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

void ObjectOpenGlModel::draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, const std::vector<LightSource> &lightSources,
    bool isDay, float k_s) {
    shader.use();

    glUniformMatrix4fv(shader.getUniformLocation("uView"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.getUniformLocation("uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(shader.getUniformLocation("uModel"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(shader.getUniformLocation("uIsDay"), isDay ? 1 : 0);
    glUniform1f(shader.getUniformLocation("uKs"), k_s);

    assert(lightSources.size() <= 5);
    glUniform1i(shader.getUniformLocation("uLightSourceCount"), lightSources.size());
    for (int i = 0; i < lightSources.size(); i++) {
        std::string index = std::to_string(i);

        std::string position = "uLightSources[" + index + "].position";
        std::string color = "uLightSources[" + index + "].color";
        std::string reflection = "uLightSources[" + index + "].reflection";
        std::string direction = "uLightSources[" + index + "].direction";

        const glm::vec3 positionVec = glm::vec3(view * glm::vec4(lightSources[i].position, 1.0f));
        glUniform3fv(shader.getUniformLocation(position.c_str()), 1, glm::value_ptr(positionVec));
        glUniform3fv(shader.getUniformLocation(color.c_str()), 1, glm::value_ptr(lightSources[i].color));
        glUniform1i(shader.getUniformLocation(reflection.c_str()), lightSources[i].reflection);

        if (lightSources[i].reflection > 0) {
            const glm::vec3 directionVec = glm::mat3(view) * lightSources[i].direction;
            glUniform3fv(shader.getUniformLocation(direction.c_str()), 1, glm::value_ptr(directionVec));
        }
    }

    glBindVertexArray(vao);
    for (const auto &[indexCount, baseIndex, materialIndex] : subMeshes) {
        glm::vec4 colorVec = glm::vec4(1.0, 0.0, 1.0, 1.0);

        if (materialIndex < materialColors.size()) {
            colorVec = materialColors[materialIndex];
        }

        glUniform4fv(shader.getUniformLocation("uObjectColor"), 1, glm::value_ptr(colorVec));

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(baseIndex));
    }
}

ObjectOpenGlModel::~ObjectOpenGlModel() {
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
