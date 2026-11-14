#pragma once

struct GLFWwindow;

/**
 * @brief Główna klasa aplikacji: okno i pętla główna.
 *
 * To jest wersja startowa (template): otwiera puste okno OpenGL 3.3 Core
 * i zamyka je po naciśnięciu Esc. Menu, scena 3D i gra dochodzą w kolejnych gałęziach.
 */
class Game {
public:
    /** @brief Inicjalizuje GLFW, okno (OpenGL 3.3 Core) i GLAD. Rzuca wyjątek przy błędzie. */
    Game();
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    /** @brief Pętla główna - wraca dopiero po zamknięciu okna. */
    void run();

private:
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* window_ = nullptr;
};
