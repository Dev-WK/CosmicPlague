#include "entities/Entity.h"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(const Mesh* mesh, const glm::vec3& position, const glm::vec3& scale,
               const glm::vec3& color)
    : position_(position), scale_(scale), color_(color), mesh_(mesh) {}

glm::mat4 Entity::modelMatrix() const {
    // Kolejność czytamy od prawej: najpierw skala, potem przesunięcie na miejsce obiektu.
    return glm::translate(glm::mat4(1.0f), position_) * glm::scale(glm::mat4(1.0f), scale_);
}

void Entity::draw(const Shader& shader) const {
    if (mesh_ == nullptr) return;

    shader.setMat4("uModel", modelMatrix());
    shader.setVec3("uColor", color_);
    mesh_->draw();
}
