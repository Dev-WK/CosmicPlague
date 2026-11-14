#include "core/Game.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stdexcept>

namespace {

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;
constexpr const char* kWindowTitle = "Cosmic Plague";

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

    window_ = glfwCreateWindow(kWindowWidth, kWindowHeight, kWindowTitle, nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window (OpenGL 3.3 Core is required)");
    }

    glfwMakeContextCurrent(window_);

    const auto loader = [](const char* name) -> void* {
        return reinterpret_cast<void*>(glfwGetProcAddress(name));
    };
    if (!gladLoadGLLoader(loader)) {
        glfwDestroyWindow(window_);
        glfwTerminate();
        throw std::runtime_error("Failed to load OpenGL functions (GLAD)");
    }

    glfwSwapInterval(1);  // VSync
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);

    int fbWidth = 0;
    int fbHeight = 0;
    glfwGetFramebufferSize(window_, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
}

Game::~Game() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

void Game::run() {
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
        }

        glClearColor(0.02f, 0.02f, 0.07f, 1.0f);  // ciemne tło kosmosu
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window_);
    }
}

void Game::framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}
