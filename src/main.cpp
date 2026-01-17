#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "models/objects/floor.h"
#include "models/objects/pine_tree.h"
#include "models/objects/rubber_ducky.h"
#include "models/objects/sphere.h"
#include "models/objects/sports_car.h"

int windowWidth = 800, windowHeight = 600;
CameraStrategy *cameraStrategy = new StationaryCameraStrategy();
glm::mat4 projection;

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

void framebufferSizeCallback(GLFWwindow *window, const int width, const int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
    projection = glm::perspective(glm::radians(60.0f), aspectRatio(), 0.1f, 100.0f);
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

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Objects
    Floor floor;
    Sphere sphere(100, 100);
    SportsCar sportsCar;
    RubberDucky rubberDucky;
    PineTree pineTree;

    // Constant transform matrices
    projection = glm::perspective(glm::radians(60.0f), aspectRatio(), 0.1f, 100.0f);

    // Light sources
    std::vector<LightSource> lightSources = {};
    lightSources.push_back(LightSource{glm::vec3(3.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0});
    lightSources.push_back(LightSource{.color = glm::vec3(3.0f, 3.0f, 3.0f), .reflection = 2});
    lightSources.push_back(LightSource{.color = glm::vec3(1.0f, 0.0f, 0.0f), .reflection = 2});
    LightSource *frontCarReflector = &lightSources[1], *rearCarReflector = &lightSources[2];

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float time = glfwGetTime();
        const glm::vec3 sportsCarPosition = glm::vec3(std::cos(time) * 3, 0.0f, std::sin(time) * 3);
        frontCarReflector->direction = glm::normalize(glm::vec3(-sportsCarPosition.z, 0, sportsCarPosition.x));;
        frontCarReflector->position = glm::vec3(0.0f, 0.5f, 0.0f) + sportsCarPosition + 2.0f * frontCarReflector->direction;
        rearCarReflector->direction = -frontCarReflector->direction;
        rearCarReflector->position = glm::vec3(0.0f, 0.5f, 0.0f) + sportsCarPosition + 2.0f * rearCarReflector->direction;

        glm::mat4 floorModel = glm::scale(identity, glm::vec3(100.0f, 100.0f, 100.0f));
        glm::mat4 sphereModel = glm::rotate(glm::translate(identity, glm::vec3(0.0f, 3.0f, 0.0f)), time,
            glm::vec3(0.5f, 0.2f, 0.0f));
        glm::mat4 sportsCarModel = glm::translate(identity, sportsCarPosition);
        sportsCarModel = glm::rotate(sportsCarModel, -time, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rubberDuckyModel = glm::rotate(identity, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 pineTreeModel = glm::translate(identity, glm::vec3(-6.0f, 0.0f, -3.0f));
        pineTreeModel = glm::scale(pineTreeModel, glm::vec3(5.0f, 5.0f, 5.0f));

        glm::mat4 view = cameraStrategy->getViewMatrix(sportsCarPosition);

        floor.draw(floorModel, view, projection, lightSources);
        sphere.draw(sphereModel, view, projection, lightSources);
        sportsCar.draw(sportsCarModel, view, projection, lightSources);
        rubberDucky.draw(rubberDuckyModel, view, projection, lightSources);
        pineTree.draw(pineTreeModel, view, projection, lightSources);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    delete cameraStrategy;
    return 0;
}