#include "ui/UIRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <stb_easy_font.h>

namespace {

// stb_easy_font zapisuje 4 wierzchołki na prostokąt (quad), po 16 bajtów każdy
// (float x, y, z + 4 bajty koloru). W Core Profile nie ma GL_QUADS, więc
// rysujemy je jako dwa trójkąty przez bufor indeksów.
constexpr int kBytesPerQuad = 4 * 16;
constexpr int kMaxQuads = 2048;
constexpr int kBufferSize = kBytesPerQuad * kMaxQuads;

}  // namespace

UIRenderer::UIRenderer(const std::string& assetsDir)
    : shader_(assetsDir + "/shaders/ui.vert", assetsDir + "/shaders/ui.frag"),
      vertexBuffer_(kBufferSize) {
    std::vector<unsigned int> indices(static_cast<std::size_t>(kMaxQuads) * 6);
    for (unsigned int q = 0; q < static_cast<unsigned int>(kMaxQuads); ++q) {
        const unsigned int base = q * 4;
        const std::size_t i = static_cast<std::size_t>(q) * 6;
        indices[i + 0] = base + 0;
        indices[i + 1] = base + 1;
        indices[i + 2] = base + 2;
        indices[i + 3] = base + 0;
        indices[i + 4] = base + 2;
        indices[i + 5] = base + 3;
    }

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, kBufferSize, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(),
                 GL_STATIC_DRAW);

    // location 0: pozycja (x, y, z); kolor z bufora ignorujemy - używamy uniformu uColor
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 16, nullptr);

    glBindVertexArray(0);
}

UIRenderer::~UIRenderer() {
    glDeleteBuffers(1, &ebo_);
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void UIRenderer::begin(int framebufferWidth, int framebufferHeight) {
    width_ = framebufferWidth;
    height_ = framebufferHeight;
    projection_ = glm::ortho(0.0f, static_cast<float>(width_), static_cast<float>(height_), 0.0f);

    depthWasEnabled_ = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
    cullWasEnabled_ = glIsEnabled(GL_CULL_FACE) == GL_TRUE;
    blendWasEnabled_ = glIsEnabled(GL_BLEND) == GL_TRUE;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_.use();
    shader_.setMat4("uProjection", projection_);
    glBindVertexArray(vao_);
}

void UIRenderer::end() {
    glBindVertexArray(0);
    if (depthWasEnabled_) glEnable(GL_DEPTH_TEST);
    if (cullWasEnabled_) glEnable(GL_CULL_FACE);
    if (!blendWasEnabled_) glDisable(GL_BLEND);
}

void UIRenderer::drawText(const std::string& text, float x, float y, float scale,
                          const glm::vec4& color) {
    if (text.empty()) return;

    std::string copy = text;  // stb_easy_font przyjmuje char*, nie const char*
    const int quads = stb_easy_font_print(0.0f, 0.0f, copy.data(), nullptr, vertexBuffer_.data(),
                                          kBufferSize);
    if (quads <= 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(quads) * kBytesPerQuad,
                    vertexBuffer_.data());

    shader_.setVec2("uOrigin", glm::vec2(x, y));
    shader_.setFloat("uScale", scale);
    shader_.setVec4("uColor", color);

    glDrawElements(GL_TRIANGLES, quads * 6, GL_UNSIGNED_INT, nullptr);
}

void UIRenderer::drawTextCentered(const std::string& text, float centerX, float y, float scale,
                                  const glm::vec4& color) {
    drawText(text, centerX - textWidth(text, scale) * 0.5f, y, scale, color);
}

float UIRenderer::textWidth(const std::string& text, float scale) const {
    std::string copy = text;
    return static_cast<float>(stb_easy_font_width(copy.data())) * scale;
}

float UIRenderer::textHeight(const std::string& text, float scale) const {
    std::string copy = text;
    return static_cast<float>(stb_easy_font_height(copy.data())) * scale;
}
