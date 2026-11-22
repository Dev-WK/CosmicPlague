#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

/**
 * @brief Program shaderowy GLSL (vertex + fragment) wczytywany z plików.
 *
 * Konstruktor kompiluje i linkuje shadery; w razie błędu rzuca std::runtime_error
 * z logiem kompilatora GLSL. Funkcje set*() zakładają, że program jest aktywny
 * (najpierw wywołaj use()).
 */
class Shader {
public:
    Shader() = default;
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    /** @brief Aktywuje program (glUseProgram). */
    void use() const;

    /** @brief Identyfikator programu OpenGL (0, jeśli shader nie został utworzony). */
    GLuint id() const { return program_; }

    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

private:
    GLint location(const std::string& name) const;

    GLuint program_ = 0;
};
