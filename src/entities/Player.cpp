#include "entities/Player.h"

#include <algorithm>

namespace {

constexpr float kSpeed = 12.0f;   // prędkość boczna statku (jednostki/s)
constexpr float kPlayerY = 0.35f; // wysokość statku nad siatką

}  // namespace

Player::Player(const Mesh* mesh)
    : Entity(mesh, glm::vec3(0.0f, kPlayerY, 0.0f), glm::vec3(0.6f, 0.6f, 0.8f),
             glm::vec3(0.2f, 0.9f, 1.0f)) {}

void Player::update(float dt) {
    // Ruch: pozycja += prędkość * dt, a statek nie może wyjechać poza siatkę.
    position_.x = std::clamp(position_.x + moveInput_ * kSpeed * dt, -kLimitX, kLimitX);
}

void Player::reset() {
    position_.x = 0.0f;
    moveInput_ = 0.0f;
}
