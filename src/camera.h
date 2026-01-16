#ifndef INC_3DGRAPHICSAPP_CAMERA_H
#define INC_3DGRAPHICSAPP_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static constexpr glm::mat4 identity(1.0f);
static constexpr glm::vec3 stationaryCameraPosition(0.0f, 3.0f, 10.0f);

class CameraStrategy {
public:
    virtual glm::mat4 getViewMatrix(const glm::vec3 &followedPosition) const = 0;
    virtual ~CameraStrategy() = default;
};

class StationaryCameraStrategy final : public CameraStrategy {
public:
    glm::mat4 getViewMatrix(const glm::vec3 &followedPosition) const override {
        return glm::translate(glm::rotate(identity, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            -stationaryCameraPosition);
    }
};

class StationaryFollowingCameraStrategy final : public CameraStrategy {
public:
    glm::mat4 getViewMatrix(const glm::vec3 &followedPosition) const override {
        return glm::lookAt(stationaryCameraPosition, followedPosition, glm::vec3(0.0f, 1.0f, 0.0f));
    }
};

class CarThirdPersonCameraStrategy final : public CameraStrategy {
public:
    glm::mat4 getViewMatrix(const glm::vec3 &followedPosition) const override {
        const glm::vec3 carDirection = glm::normalize(glm::vec3(-followedPosition.z, 0, followedPosition.x));
        const glm::vec3 cameraPosition = followedPosition + glm::vec3(0.0f, 2.0f, 0.0f) - 5.0f * carDirection;
        const glm::vec3 cameraDirection = followedPosition + glm::vec3(0.0f, 1.0f, 0.0f);
        return glm::lookAt(cameraPosition, cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f));
    }
};

#endif //INC_3DGRAPHICSAPP_CAMERA_H