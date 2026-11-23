#pragma once

#include "core/Shader.h"
#include "entities/Player.h"
#include "render/Camera.h"
#include "render/GridRenderer.h"
#include "render/Mesh.h"
#include "world/InputState.h"

#include <memory>
#include <string>

/**
 * @brief Świat gry: siatka, kamera, statek gracza i rysowanie sceny 3D.
 *
 * World nic nie wie o oknie ani o klawiaturze - dostaje tylko InputState i czas dt.
 * Game (okno, pętla główna, ekrany) wywołuje co klatkę update() i render().
 *
 * Na razie w świecie jest tylko statek poruszany w lewo i w prawo. Wrogowie, strzelanie,
 * sektory i checkpointy to zadania na kolejne etapy.
 */
class World {
public:
    /** @param assetsDir katalog z zasobami (shadery czytane z <assetsDir>/shaders/) */
    explicit World(const std::string& assetsDir);

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    /** @brief Zaczyna grę od nowa: statek wraca na środek. */
    void start();

    /** @brief Aktualizacja świata o dt sekund. */
    void update(float dt, const InputState& input);

    /** @brief Rysuje scenę 3D (siatka i statek) do aktualnie aktywnego bufora. */
    void render(int framebufferWidth, int framebufferHeight);

private:
    // Zasoby GPU (kolejność ma znaczenie: shadery i modele powstają przed obiektami).
    Shader modelShader_;  // oświetlone modele
    Shader flatShader_;   // jednokolorowe linie siatki
    Camera camera_;
    GridRenderer grid_;
    Mesh shipMesh_;

    // Obiekty gry.
    std::unique_ptr<Player> player_;
};
