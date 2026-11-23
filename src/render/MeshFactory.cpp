#include "render/MeshFactory.h"

#include <glm/glm.hpp>

#include <utility>
#include <vector>

namespace {

/**
 * Dodaje trójkąt z jedną normalną dla całej ściany (cieniowanie płaskie).
 * Kolejność wierzchołków jest poprawiana tak, by normalna zawsze patrzyła
 * na zewnątrz bryły - dzięki temu nie trzeba ręcznie pilnować kierunku obrotu.
 * Działa dla brył wypukłych; center to dowolny punkt wewnątrz bryły.
 */
void addTriangle(std::vector<Vertex>& out, glm::vec3 a, glm::vec3 b, glm::vec3 c,
                 const glm::vec3& center) {
    glm::vec3 normal = glm::cross(b - a, c - a);
    const glm::vec3 faceCenter = (a + b + c) / 3.0f;
    if (glm::dot(normal, faceCenter - center) < 0.0f) {
        std::swap(b, c);
        normal = -normal;
    }
    normal = glm::normalize(normal);
    out.push_back({a, normal});
    out.push_back({b, normal});
    out.push_back({c, normal});
}

}  // namespace

namespace meshes {

Mesh createShip() {
    // Cztery punkty: dziób (nose), lewe i prawe skrzydło oraz grzbiet (top) z tyłu.
    // To czworościan, czyli najprostsza bryła 3D: cztery trójkątne ściany.
    const glm::vec3 nose(0.0f, 0.0f, -1.0f);
    const glm::vec3 left(-1.0f, 0.0f, 1.0f);
    const glm::vec3 right(1.0f, 0.0f, 1.0f);
    const glm::vec3 top(0.0f, 0.7f, 0.4f);
    const glm::vec3 center(0.0f, 0.175f, 0.35f);  // punkt wewnątrz bryły (średnia czterech wierzchołków)

    std::vector<Vertex> v;
    addTriangle(v, nose, left, top, center);    // lewa ściana
    addTriangle(v, nose, top, right, center);   // prawa ściana
    addTriangle(v, left, right, top, center);   // tył
    addTriangle(v, nose, right, left, center);  // spód
    return Mesh(v);
}

}  // namespace meshes
