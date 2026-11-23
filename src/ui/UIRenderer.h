#pragma once

#include "core/Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

/**
 * @brief Rysowanie interfejsu 2D (menu, w przyszłości: wynik, sektor, życia).
 *
 * Tekst jest generowany przez stb_easy_font (czcionka wektorowa wbudowana w nagłówek,
 * bez plików czcionek). Ograniczenie: tylko znaki ASCII, bez polskich ogonków.
 *
 * Użycie w klatce - zawsze na końcu, po scenie 3D:
 * @code
 * ui.begin(framebufferWidth, framebufferHeight);
 * ui.drawText("SCORE 0", 20, 20, 3.0f, {1, 1, 1, 1});
 * ui.end();
 * @endcode
 *
 * Współrzędne w pikselach, początek (0,0) w lewym górnym rogu okna, oś Y rośnie w dół.
 */
class UIRenderer {
public:
    /** @param assetsDir katalog z zasobami; shadery czytane z <assetsDir>/shaders/ui.{vert,frag} */
    explicit UIRenderer(const std::string& assetsDir);
    ~UIRenderer();

    UIRenderer(const UIRenderer&) = delete;
    UIRenderer& operator=(const UIRenderer&) = delete;

    /** @brief Zaczyna rysowanie UI: wyłącza test głębi i culling, włącza blending. */
    void begin(int framebufferWidth, int framebufferHeight);

    /** @brief Kończy rysowanie UI i przywraca poprzedni stan testu głębi, cullingu i blendingu. */
    void end();

    /** @brief Rysuje tekst z lewym górnym rogiem w (x, y). */
    void drawText(const std::string& text, float x, float y, float scale, const glm::vec4& color);

    /** @brief Rysuje tekst wyśrodkowany poziomo względem centerX. */
    void drawTextCentered(const std::string& text, float centerX, float y, float scale,
                          const glm::vec4& color);

    float textWidth(const std::string& text, float scale) const;
    float textHeight(const std::string& text, float scale) const;

    int width() const { return width_; }
    int height() const { return height_; }

private:
    Shader shader_;
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLuint ebo_ = 0;

    glm::mat4 projection_{1.0f};
    int width_ = 0;
    int height_ = 0;

    bool depthWasEnabled_ = false;
    bool cullWasEnabled_ = false;
    bool blendWasEnabled_ = false;

    std::vector<char> vertexBuffer_;
};
