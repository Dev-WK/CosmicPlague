#include "render/Mesh.h"

#include <utility>

Mesh::Mesh(const std::vector<Vertex>& vertices)
    : vertexCount_(static_cast<GLsizei>(vertices.size())) {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
                 vertices.data(), GL_STATIC_DRAW);

    // location 0: pozycja, location 1: normalna (shader model.vert)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const void*>(sizeof(glm::vec3)));

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if (vbo_ != 0) glDeleteBuffers(1, &vbo_);
    if (vao_ != 0) glDeleteVertexArrays(1, &vao_);
}

Mesh::Mesh(Mesh&& other) noexcept
    : vao_(std::exchange(other.vao_, 0)),
      vbo_(std::exchange(other.vbo_, 0)),
      vertexCount_(std::exchange(other.vertexCount_, 0)) {}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        if (vbo_ != 0) glDeleteBuffers(1, &vbo_);
        if (vao_ != 0) glDeleteVertexArrays(1, &vao_);
        vao_ = std::exchange(other.vao_, 0);
        vbo_ = std::exchange(other.vbo_, 0);
        vertexCount_ = std::exchange(other.vertexCount_, 0);
    }
    return *this;
}

void Mesh::draw() const {
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
    glBindVertexArray(0);
}
