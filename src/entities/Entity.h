#pragma once

#include "core/Shader.h"
#include "render/Mesh.h"

#include <glm/glm.hpp>

/**
 * @brief Klasa bazowa każdego obiektu w grze (na razie tylko statek gracza).
 *
 * To jest "archetyp" gry: obiekt ma położenie, skalę, kolor, model 3D (Mesh, tylko wskaźnik -
 * właścicielem modeli jest World) oraz wirtualną funkcję update(dt), w której klasa pochodna
 * definiuje swoje zachowanie. Nowe rodzaje obiektów (wrogowie, pociski) będą dziedziczyć po Entity.
 */
class Entity {
public:
    Entity(const Mesh* mesh, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& color);
    virtual ~Entity() = default;

    /** @brief Aktualizacja zachowania obiektu o dt sekund. */
    virtual void update(float dt) = 0;

    /** @brief Macierz modelu: przesunięcie * skala. Klasy pochodne mogą dołożyć własny obrót (np. przechył statku). */
    virtual glm::mat4 modelMatrix() const;

    /** @brief Rysuje obiekt aktywnym shaderem modelu (ustawia uModel i uColor). */
    void draw(const Shader& shader) const;

    const glm::vec3& position() const { return position_; }

protected:
    glm::vec3 position_;
    glm::vec3 scale_;  // skala modelu 3D (model ma rozmiar od -1 do 1, więc skala = połowa rozmiaru)
    glm::vec3 color_;

private:
    const Mesh* mesh_;
};
