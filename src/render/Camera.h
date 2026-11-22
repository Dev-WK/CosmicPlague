#pragma once

#include <glm/glm.hpp>

/**
 * @brief Nieruchoma kamera za plecami statku, skierowana pod kątem w głąb sceny.
 *
 * Układ współrzędnych świata: płaszczyzna gry to XZ (Y = 0 to siatka), oś X biegnie
 * w prawo, oś -Z w stronę horyzontu, a Y w górę. Statek gracza stoi w z = 0.
 */
class Camera {
public:
    /** @brief Aktualizuje proporcje obrazu (szerokość / wysokość) po zmianie rozmiaru okna. */
    void setAspect(float aspect) { aspect_ = aspect; }

    /** @brief Macierz widoku - glm::lookAt (kamera nad płaszczyzną, patrzy na horyzont). */
    glm::mat4 view() const;

    /** @brief Macierz rzutowania perspektywicznego - glm::perspective. */
    glm::mat4 projection() const;

    const glm::vec3& position() const { return position_; }

private:
    glm::vec3 position_{0.0f, 3.5f, 8.0f};  // nad siatką i za statkiem
    glm::vec3 target_{0.0f, 0.0f, -15.0f};  // punkt, na który patrzy kamera
    float fovDegrees_ = 55.0f;
    float aspect_ = 16.0f / 9.0f;
    float nearPlane_ = 0.1f;
    float farPlane_ = 150.0f;
};
