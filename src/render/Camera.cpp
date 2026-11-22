#include "render/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::view() const {
    return glm::lookAt(position_, target_, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::projection() const {
    return glm::perspective(glm::radians(fovDegrees_), aspect_, nearPlane_, farPlane_);
}
