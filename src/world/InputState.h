#pragma once

/**
 * @brief Stan sterowania w jednej klatce. Game zamienia klawisze na ten struct,
 * a World nic nie wie o klawiaturze ani o GLFW (łatwiej testować, łatwiej dodać np. pada).
 */
struct InputState {
    float moveX = 0.0f;  ///< -1 = lewo, 0 = brak ruchu, 1 = prawo
};
