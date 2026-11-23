#include "ui/MenuScreen.h"

#include "ui/UIRenderer.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <algorithm>

MenuScreen::MenuScreen()
    : items_{{"START GAME", MenuAction::StartGame}, {"QUIT", MenuAction::Quit}} {}

void MenuScreen::onKey(int key, int action) {
    if (action == GLFW_RELEASE) return;

    switch (key) {
        case GLFW_KEY_UP:
        case GLFW_KEY_W:
            move(-1);
            break;
        case GLFW_KEY_DOWN:
        case GLFW_KEY_S:
            move(+1);
            break;
        case GLFW_KEY_ENTER:
        case GLFW_KEY_KP_ENTER:
        case GLFW_KEY_SPACE:
            if (action == GLFW_PRESS) {
                pending_ = items_[selected_].action;
            }
            break;
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) {
                pending_ = MenuAction::Quit;
            }
            break;
        default:
            break;
    }
}

MenuAction MenuScreen::takeAction() {
    const MenuAction action = pending_;
    pending_ = MenuAction::None;
    return action;
}

void MenuScreen::move(int direction) {
    // Zawijanie: po ostatniej pozycji wracamy na pierwszą i odwrotnie.
    const int count = static_cast<int>(items_.size());
    const int index = (static_cast<int>(selected_) + direction + count) % count;
    selected_ = static_cast<std::size_t>(index);
}

void MenuScreen::render(UIRenderer& ui) const {
    const float w = static_cast<float>(ui.width());
    const float h = static_cast<float>(ui.height());
    const float u = h / 720.0f;  // interfejs skaluje się razem z wysokością okna
    const float cx = w * 0.5f;

    const glm::vec4 green(0.45f, 1.00f, 0.15f, 1.0f);
    const glm::vec4 dimBlue(0.35f, 0.50f, 0.65f, 1.0f);

    // Tytuł
    const std::string title = "COSMIC PLAGUE";
    const float titleScale = std::min(8.0f * u, 0.9f * w / ui.textWidth(title, 1.0f));
    ui.drawTextCentered(title, cx, h * 0.18f, titleScale, green);

    // Pozycje menu: zaznaczona jest zielona i otoczona znakami "> <".
    const float itemScale = 4.0f * u;
    float y = h * 0.45f;
    for (std::size_t i = 0; i < items_.size(); ++i) {
        if (i == selected_) {
            ui.drawTextCentered("> " + items_[i].label + " <", cx, y, itemScale, green);
        } else {
            ui.drawTextCentered(items_[i].label, cx, y, itemScale, dimBlue);
        }
        y += 80.0f * u;
    }

    // Podpowiedź sterowania
    ui.drawTextCentered("W/S OR ARROWS - SELECT     ENTER - CONFIRM     ESC - QUIT", cx,
                        h - 50.0f * u, 2.0f * u, dimBlue);
}
