#include "sphere.h"

#include <cmath>
#include <vector>
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

static constexpr char vertexShader[] = {
    #embed "sphere.vert"
    , 0
};

static constexpr char fragmentShader[] = {
    #embed "sphere.frag"
    , 0
};

Sphere::Sphere(const int stackCount, const int sectorCount) : shader(vertexShader, fragmentShader) {
    // Calculate vertices array
    // Since r = 1, normal vectors are equal to vertices coords
    std::vector<float> vertices = {};
    // std::vector<float> texCoords = {};

    const float sectorStep = 2 * M_PI / sectorCount;
    const float stackStep = M_PI / stackCount;

    for (int i = 0; i <= stackCount; ++i) {
        const float stackAngle = M_PI / 2 - i * stackStep;
        const float xy = cosf(stackAngle);
        float z = sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            const float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // float s = (float)j / sectorCount;
            // float t = (float)i / stackCount;
            // texCoords.push_back(s);
            // texCoords.push_back(t);
        }
    }

    // Calculate indices array
    std::vector<unsigned int> indices = {};
    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1); // beginning of current stack
        int k2 = k1 + sectorCount + 1; // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != stackCount - 1) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    indicesCount = indices.size();

    // Initialize OpenGL objects
    shader.use();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
}

void Sphere::draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection) {
    shader.use();

    glUniformMatrix4fv(shader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
}

Sphere::~Sphere() {
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
