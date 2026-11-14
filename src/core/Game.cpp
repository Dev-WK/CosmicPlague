#include "core/Game.h"

#include "ui/UIRenderer.h"
#include "world/World.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <stdexcept>

#ifndef CP_ASSETS_DIR
#define CP_ASSETS_DIR "assets"
#endif

namespace {

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;
constexpr const char* kWindowTitle = "Cosmic Plague";

// Najdłuższy krok symulacji. Po przeciągnięciu okna dt bywa ogromne,
// a bez limitu statek "skoczyłby" od razu na drugą stronę ekranu.
constexpr double kMaxFrameTime = 1.0 / 30.0;

}  // namespace

Game::Game() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // wymagane przez macOS dla Core Profile
#endif
    glfwWindowHint(GLFW_SAMPLES, 4);  // MSAA - wygładza linie siatki

    window_ = glfwCreateWindow(kWindowWidth, kWindowHeight, kWindowTitle, nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window (OpenGL 3.3 Core is required)");
    }

    try {
        glfwMakeContextCurrent(window_);

        const auto loader = [](const char* name) -> void* {
            return reinterpret_cast<void*>(glfwGetProcAddress(name));
        };
        if (!gladLoadGLLoader(loader)) {
            throw std::runtime_error("Failed to load OpenGL functions (GLAD)");
        }

        glfwSwapInterval(1);  // VSync
        glfwSetWindowUserPointer(window_, this);
        glfwSetKeyCallback(window_, keyCallback);
        glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);

        int fbWidth = 0;
        int fbHeight = 0;
        glfwGetFramebufferSize(window_, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
        glEnable(GL_DEPTH_TEST);

        ui_ = std::make_unique<UIRenderer>(CP_ASSETS_DIR);
        world_ = std::make_unique<World>(CP_ASSETS_DIR);
    } catch (...) {
        world_.reset();
        ui_.reset();
        glfwDestroyWindow(window_);
        glfwTerminate();
        throw;
    }
}

Game::~Game() {
    // zasoby OpenGL muszą zostać zwolnione, zanim zniknie kontekst
    world_.reset();
    ui_.reset();
    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

void Game::run() {
    double previous = glfwGetTime();

    while (state_ != State::Quit && !glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        // Okno zminimalizowane: nie rysujemy i nie mielimy procesora w pustej pętli.
        int fbWidth = 0;
        int fbHeight = 0;
        glfwGetFramebufferSize(window_, &fbWidth, &fbHeight);
        if (fbWidth == 0 || fbHeight == 0) {
            glfwWaitEventsTimeout(0.1);
            previous = glfwGetTime();
            continue;
        }

        const double now = glfwGetTime();
        const double dt = std::min(now - previous, kMaxFrameTime);
        previous = now;

        update(dt);
        render();
        glfwSwapBuffers(window_);
    }
}

void Game::keyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
    auto* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game) {
        game->handleKey(key, action);
    }
}

void Game::framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

void Game::handleKey(int key, int action) {
    switch (state_) {
        case State::Menu:
            menu_.onKey(key, action);
            applyMenuAction(menu_.takeAction());
            break;
        case State::Playing:
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                state_ = State::Menu;  // Esc w grze wraca do menu
            }
            break;
        case State::Quit:
            break;
    }
}

void Game::applyMenuAction(MenuAction action) {
    switch (action) {
        case MenuAction::StartGame:
            world_->start();
            state_ = State::Playing;
            break;
        case MenuAction::Quit:
            state_ = State::Quit;
            break;
        case MenuAction::None:
            break;
    }
}

InputState Game::readInput() const {
    const auto pressed = [this](int key) { return glfwGetKey(window_, key) == GLFW_PRESS; };

    InputState input;
    if (pressed(GLFW_KEY_LEFT) || pressed(GLFW_KEY_A)) input.moveX -= 1.0f;
    if (pressed(GLFW_KEY_RIGHT) || pressed(GLFW_KEY_D)) input.moveX += 1.0f;
    return input;
}

void Game::update(double dt) {
    if (state_ != State::Playing) return;

    world_->update(static_cast<float>(dt), readInput());
}

void Game::render() {
    int fbWidth = 0;
    int fbHeight = 0;
    glfwGetFramebufferSize(window_, &fbWidth, &fbHeight);

    glClearColor(0.02f, 0.02f, 0.07f, 1.0f);  // ciemne tło kosmosu
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (state_ == State::Playing) {
        world_->render(fbWidth, fbHeight);
    }

    // Interfejs zawsze na końcu, na wierzchu sceny 3D.
    ui_->begin(fbWidth, fbHeight);
    if (state_ == State::Playing) {
        const float u = static_cast<float>(fbHeight) / 720.0f;
        ui_->drawTextCentered("A/D OR ARROWS - MOVE     ESC - MENU", static_cast<float>(fbWidth) * 0.5f,
                              static_cast<float>(fbHeight) - 40.0f * u, 2.0f * u,
                              glm::vec4(0.35f, 0.50f, 0.65f, 1.0f));
    } else {
        menu_.render(*ui_);
    }
    ui_->end();
}
