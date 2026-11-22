#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

/** @brief Wierzchołek modelu: pozycja i normalna (normalna jest potrzebna do oświetlenia Phonga). */
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

/**
 * @brief Model 3D wgrany na kartę graficzną (VAO + VBO).
 *
 * Mesh nie zna swojego położenia - gdzie i jak go narysować, mówi macierz modelu
 * ustawiana w shaderze (patrz Entity::draw). Dzięki temu jeden Mesh może służyć
 * wielu obiektom naraz (np. wszystkim wrogom).
 *
 * Wierzchołki rysowane są jako trójkąty (GL_TRIANGLES), po 3 na ścianę.
 */
class Mesh {
public:
    explicit Mesh(const std::vector<Vertex>& vertices);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    /** @brief Rysuje model aktualnie aktywnym shaderem. */
    void draw() const;

private:
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLsizei vertexCount_ = 0;
};
