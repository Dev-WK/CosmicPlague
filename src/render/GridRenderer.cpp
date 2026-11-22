#include "render/GridRenderer.h"

#include <glm/glm.hpp>

#include <vector>

namespace {

constexpr float kHalfWidth = 7.5f;   // siatka sięga od x = -7.5 do 7.5
constexpr float kNearZ = 10.0f;      // początek siatki (za kamerą, żeby nie było widać jej końca)
constexpr float kFarZ = -60.0f;      // koniec siatki (przy horyzoncie, dalej linie zlewałyby się w jedną plamę)
constexpr int kCrossLines = 28;      // tyle linii poprzecznych pokrywa odcinek od kNearZ do kFarZ

}  // namespace

GridRenderer::GridRenderer() {
    std::vector<glm::vec3> vertices;

    // Linie podłużne: co kSpacing w poprzek, każda to dwa punkty (początek i koniec).
    for (float x = -kHalfWidth; x <= kHalfWidth + 0.001f; x += kSpacing) {
        vertices.emplace_back(x, 0.0f, kNearZ);
        vertices.emplace_back(x, 0.0f, kFarZ);
    }

    // Linie poprzeczne: co kSpacing w głąb sceny.
    for (int i = 0; i < kCrossLines; ++i) {
        const float z = kNearZ - static_cast<float>(i) * kSpacing;
        vertices.emplace_back(-kHalfWidth, 0.0f, z);
        vertices.emplace_back(kHalfWidth, 0.0f, z);
    }
    vertexCount_ = static_cast<GLsizei>(vertices.size());

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3)),
                 vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glBindVertexArray(0);
}

GridRenderer::~GridRenderer() {
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void GridRenderer::draw(const Shader& shader) const {
    glLineWidth(2.0f);  // grubsze linie; sterownik może zignorować wartość > 1

    shader.setMat4("uModel", glm::mat4(1.0f));  // siatka stoi w miejscu, więc macierz jednostkowa

    glBindVertexArray(vao_);
    glDrawArrays(GL_LINES, 0, vertexCount_);
    glBindVertexArray(0);
}
