#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "shaders/shader.h"

int windowWidth = 800, windowHeight = 600;

void framebuffer_size_callback(GLFWwindow *window, const int width, const int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float aspectRatio() {
    return static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

int main() {
    // Window creation
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "3D Scene", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (gladLoadGL(glfwGetProcAddress) == 0)
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, windowWidth, windowHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    stbi_set_flip_vertically_on_load(true);

    // Data
    constexpr float vertices1[] = {
        // positions            // colors
         1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 0.0f, // v0 - red
        -1.0f, -1.0f,  1.0f,    0.0f, 1.0f, 0.0f, // v1 - green
        -1.0f,  1.0f, -1.0f,    0.0f, 0.0f, 1.0f, // v2 - blue
         1.0f, -1.0f, -1.0f,    1.0f, 1.0f, 0.0f  // v3 - yellow
    };

    constexpr unsigned int indices1[] = {
        0, 1, 2,
        0, 3, 1,
        0, 2, 3,
        1, 3, 2
    };

    // OpenGL data objects
    unsigned int VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

    unsigned int VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    unsigned int EBO1;
    glGenBuffers(1, &EBO1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Shaders
    const Shader shader("shaders/vertex/example.vert", "shaders/fragment/example.frag");
    shader.use();

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Constant transform matrices
    glm::mat4 viewMat(1.0f);
    viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -3.0f));
    glUniformMatrix4fv(shader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMat));

    glm::mat4 projectionMat = glm::perspective(glm::radians(90.0f), aspectRatio(), 0.1f, 100.0f);
    glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projectionMat));

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelMat(1.0f);
        modelMat = glm::rotate(modelMat, static_cast<float>(glfwGetTime()), glm::vec3(0.5f, 0.2f, 0.0f));
        glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelMat));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
        glDrawElements(GL_TRIANGLES, sizeof(indices1), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}