#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "objects/rubber_ducky/rubber_ducky.h"
#include "objects/sphere/sphere.h"
#include "objects/sports_car/sports_car.h"

int windowWidth = 800, windowHeight = 600;
CameraStrategy *cameraStrategy = new StationaryCameraStrategy();

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
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        delete cameraStrategy;
        cameraStrategy = new StationaryCameraStrategy();
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        delete cameraStrategy;
        cameraStrategy = new StationaryFollowingCameraStrategy();
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        delete cameraStrategy;
        cameraStrategy = new CarThirdPersonCameraStrategy();
    }
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

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Objects
    Sphere sphere(100, 100);
    SportsCar sportsCar;
    RubberDucky rubberDucky;

    // Constant transform matrices
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRatio(), 0.1f, 100.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float time = glfwGetTime();
        const glm::vec3 sportsCarPosition = glm::vec3(std::cos(time) * 3, 0.0f, std::sin(time) * 3);

        glm::mat4 sphereModel = glm::rotate(glm::translate(identity, glm::vec3(0.0f, 3.0f, 0.0f)), time,
            glm::vec3(0.5f, 0.2f, 0.0f));
        glm::mat4 sportsCarModel = glm::translate(identity, sportsCarPosition);
        sportsCarModel = glm::rotate(sportsCarModel, -time, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rubberDuckyModel = glm::rotate(identity, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 view = cameraStrategy->getViewMatrix(sportsCarPosition);

        sphere.draw(sphereModel, view, projection);
        sportsCar.draw(sportsCarModel, view, projection);
        rubberDucky.draw(rubberDuckyModel, view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    delete cameraStrategy;
    return 0;
}