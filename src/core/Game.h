#pragma once

#include "ui/MenuScreen.h"
#include "world/InputState.h"

#include <memory>

struct GLFWwindow;
class UIRenderer;
class World;

/**
 * @brief Główna klasa aplikacji: okno, pętla główna i przełączanie ekranów (menu / gra).
 *
 * Game odpowiada za rzeczy "dookoła" rozgrywki: okno, klawisze i stany.
 * Samą sceną 3D zajmuje się World.
 */
class Game {
public:
    /** @brief Inicjalizuje GLFW, okno (OpenGL 3.3 Core), GLAD i interfejs. Rzuca wyjątek przy błędzie. */
    Game();
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    /** @brief Pętla główna - wraca dopiero po zamknięciu gry. */
    void run();

private:
    enum class State { Menu, Playing, Quit };

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    void handleKey(int key, int action);
    void applyMenuAction(MenuAction action);

    InputState readInput() const;
    void update(double dt);
    void render();

    GLFWwindow* window_ = nullptr;
    std::unique_ptr<UIRenderer> ui_;
    std::unique_ptr<World> world_;
    MenuScreen menu_;

    State state_ = State::Menu;
};
