#pragma once

#include "render/Mesh.h"

/**
 * @brief Proceduralne modele Low-Poly generowane w kodzie (bez plików .obj).
 *
 * Każdy model mieści się w sześcianie [-1, 1], a docelowy rozmiar nadaje mu skala obiektu.
 * Ściany mają własne normalne (cieniowanie płaskie) - to daje charakterystyczny wygląd low-poly.
 *
 * Gdy zespół doda wczytywanie .obj, nowa funkcja może zwracać Mesh tak samo jak ta.
 */
namespace meshes {

/** @brief Statek gracza: prosty klin z czterech trójkątów, dziób skierowany w stronę -Z (w głąb ekranu). */
Mesh createShip();

}  // namespace meshes
