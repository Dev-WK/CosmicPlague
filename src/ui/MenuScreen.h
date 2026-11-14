#pragma once

#include <cstddef>
#include <string>
#include <vector>

class UIRenderer;

/** @brief Akcja wybrana przez gracza w menu głównym. */
enum class MenuAction {
    None,       ///< nic nie wybrano
    StartGame,  ///< start gry
    Quit        ///< wyjście z gry
};

/**
 * @brief Menu główne: START GAME / QUIT.
 *
 * Klasa nie zna GLFW-owego okna ani stanu gry - dostaje zdarzenia klawiatury
 * przez onKey(), a wybraną akcję oddaje przez takeAction(). Dzięki temu da się
 * ją rozbudować (np. o ekran opcji) bez ruszania pętli głównej.
 *
 * Sterowanie: W/S lub strzałki - wybór, Enter/Spacja - zatwierdzenie, Esc - wyjście.
 */
class MenuScreen {
public:
    MenuScreen();

    /** @brief Obsługa klawisza (wartości key/action jak w callbacku GLFW). */
    void onKey(int key, int action);

    /** @brief Zwraca wybraną akcję i zeruje ją (kolejne wywołanie zwróci None). */
    MenuAction takeAction();

    /** @brief Rysuje menu. */
    void render(UIRenderer& ui) const;

private:
    struct Item {
        std::string label;
        MenuAction action;
    };

    void move(int direction);

    std::vector<Item> items_;
    std::size_t selected_ = 0;
    MenuAction pending_ = MenuAction::None;
};
