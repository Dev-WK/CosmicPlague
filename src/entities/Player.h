#pragma once

#include "entities/Entity.h"

/**
 * @brief Statek gracza: porusza się tylko w lewo i w prawo.
 */
class Player : public Entity {
public:
    /** @brief Statek nie wyjedzie poza ten zakres w osi X. */
    static constexpr float kLimitX = 6.0f;

    explicit Player(const Mesh* mesh);

    /** @brief Kierunek ruchu: -1 = lewo, 0 = stoi, 1 = prawo. Ustawiane co klatkę z klawiatury. */
    void setMoveInput(float direction) { moveInput_ = direction; }

    void update(float dt) override;

    /** @brief Ustawia statek na środku (początek nowej gry). */
    void reset();

private:
    float moveInput_ = 0.0f;
};
