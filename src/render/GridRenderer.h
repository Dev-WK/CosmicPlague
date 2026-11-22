#pragma once

#include "core/Shader.h"

#include <glad/glad.h>

/**
 * @brief Siatka rysowana w prawdziwych współrzędnych 3D na płaszczyźnie Y = 0.
 *
 * Składa się z linii biegnących w głąb ekranu i linii poprzecznych. Perspektywa sama
 * powiększa linie w miarę zbliżania do kamery. Siatka jest generowana raz w konstruktorze
 * i na razie stoi w miejscu (animacja ruchu do przodu to zadanie na kolejne etapy).
 */
class GridRenderer {
public:
    /** @brief Odstęp między liniami (jednostki świata). */
    static constexpr float kSpacing = 2.5f;

    GridRenderer();
    ~GridRenderer();

    GridRenderer(const GridRenderer&) = delete;
    GridRenderer& operator=(const GridRenderer&) = delete;

    /**
     * @brief Rysuje siatkę.
     * @param shader aktywny shader flat (ustawione uView, uProjection, uColor); uModel ustawia ta funkcja
     */
    void draw(const Shader& shader) const;

private:
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLsizei vertexCount_ = 0;  // liczba wierzchołków wszystkich linii (po dwa na linię)
};
