#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "camera.h"
#include "models/objects/floor.h"
#include "models/objects/mirror_surface.h"
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // Objects
    Floor floor;
    Sphere sphere(100, 100);
    SportsCar sportsCar;
    RubberDucky rubberDucky;
    PineTree pineTree;
    MirrorSurface mirrorSurface;

    // Constant transform matrices
    projection = glm::perspective(glm::radians(60.0f), aspectRatio(), 0.1f, 100.0f);

    // Light sources
    std::vector<LightSource> lightSources = {};
    lightSources.push_back(LightSource{glm::vec3(3.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0});
    lightSources.push_back(LightSource{.color = glm::vec3(3.0f, 3.0f, 3.0f), .reflection = 2});
    lightSources.push_back(LightSource{.color = glm::vec3(1.0f, 0.0f, 0.0f), .reflection = 2});
    lightSources.push_back(LightSource{glm::vec3(-4.0f, 2.0f, 1.0f), glm::vec3(3.0f, 3.0f, 0.0f), 5, glm::vec3(1.0f, -1.0f, 0.0f)});
    LightSource *frontCarReflector = &lightSources[1], *rearCarReflector = &lightSources[2];
    float *adjustableReflectorDirectionVector = glm::value_ptr(lightSources[3].direction);

    glm::vec3 carFrontReflectorTwist(0.0f);
    float *carFrontReflectorTwistVectorPointer = glm::value_ptr(carFrontReflectorTwist);

    bool isDay = true;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Reflector adjustment:");
        ImGui::SliderFloat3("Yellow one", adjustableReflectorDirectionVector, -1.0f, 1.0f);
        ImGui::SliderFloat3("Car front reflector", carFrontReflectorTwistVectorPointer, -1.0f, 1.0f);
        ImGui::End();

        ImGui::Begin("Daylight cycle:");
        ImGui::Checkbox("Day", &isDay);
        ImGui::End();

        processInput(window);

        const float time = glfwGetTime();
        const glm::vec3 sportsCarPosition = glm::vec3(std::cos(time) * 3, 0.0f, std::sin(time) * 3);
        const glm::vec3 mirrorPosition = glm::vec3(std::sin(time / 2) * 3, 0.5f, -6.0f);

        frontCarReflector->direction = glm::normalize(glm::vec3(-sportsCarPosition.z, 0, sportsCarPosition.x) + carFrontReflectorTwist);
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
        glm::mat4 mirrorModel = glm::translate(identity, mirrorPosition);

        glm::mat4 view = cameraStrategy->getViewMatrix(sportsCarPosition);

        if (isDay) {
            glClearColor(0.5f, 0.6f, 0.7f, 1.0f); // Fog color
        } else {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }

        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        mirrorSurface.draw(mirrorModel, view, projection, lightSources, isDay);

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilMask(0x00);

        glm::mat4 reflectedView = identity;
        reflectedView = glm::translate(reflectedView, mirrorPosition);
        reflectedView = glm::scale(reflectedView, glm::vec3(1.0f, 1.0f, -1.0f));
        reflectedView = glm::translate(reflectedView, -mirrorPosition);
        reflectedView = view * reflectedView;

        glCullFace(GL_FRONT);

        sphere.draw(sphereModel, reflectedView, projection, lightSources, isDay);
        sportsCar.draw(sportsCarModel, reflectedView, projection, lightSources, isDay);
        rubberDucky.draw(rubberDuckyModel, reflectedView, projection, lightSources, isDay);
        pineTree.draw(pineTreeModel, reflectedView, projection, lightSources, isDay);

        glCullFace(GL_BACK);
        glDisable(GL_STENCIL_TEST);

        floor.draw(floorModel, view, projection, lightSources, isDay);
        sphere.draw(sphereModel, view, projection, lightSources, isDay);
        sportsCar.draw(sportsCarModel, view, projection, lightSources, isDay);
        rubberDucky.draw(rubberDuckyModel, view, projection, lightSources, isDay);
        pineTree.draw(pineTreeModel, view, projection, lightSources, isDay);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        mirrorSurface.draw(mirrorModel, view, projection, lightSources, isDay);

        glDisable(GL_BLEND);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    delete cameraStrategy;
    return 0;
}