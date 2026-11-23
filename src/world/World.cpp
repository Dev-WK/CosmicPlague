#include "world/World.h"

#include "render/MeshFactory.h"

#include <glad/glad.h>

#include <algorithm>

namespace {

const glm::vec3 kGridColor(0.25f, 1.00f, 0.30f);  // zielona siatka
const glm::vec3 kLightDir(-0.4f, 1.0f, 0.6f);     // kierunek DO światła (z góry, trochę z lewej i z tyłu)

}  // namespace

World::World(const std::string& assetsDir)
    : modelShader_(assetsDir + "/shaders/model.vert", assetsDir + "/shaders/model.frag"),
      flatShader_(assetsDir + "/shaders/flat.vert", assetsDir + "/shaders/flat.frag"),
      shipMesh_(meshes::createShip()) {
    player_ = std::make_unique<Player>(&shipMesh_);
}

void World::start() { player_->reset(); }

void World::update(float dt, const InputState& input) {
    player_->setMoveInput(input.moveX);
    player_->update(dt);
}

void World::render(int framebufferWidth, int framebufferHeight) {
    camera_.setAspect(static_cast<float>(framebufferWidth) /
                      static_cast<float>(std::max(framebufferHeight, 1)));
    glEnable(GL_DEPTH_TEST);

    // Siatka (shader flat).
    flatShader_.use();
    flatShader_.setMat4("uView", camera_.view());
    flatShader_.setMat4("uProjection", camera_.projection());
    flatShader_.setVec3("uColor", kGridColor);
    grid_.draw(flatShader_);

    // Statek (shader z oświetleniem).
    modelShader_.use();
    modelShader_.setMat4("uView", camera_.view());
    modelShader_.setMat4("uProjection", camera_.projection());
    modelShader_.setVec3("uLightDir", glm::normalize(kLightDir));
    player_->draw(modelShader_);
}
