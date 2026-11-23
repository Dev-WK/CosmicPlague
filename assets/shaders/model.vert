#version 330 core

layout (location = 0) in vec3 aPos;      // pozycja wierzchołka w układzie modelu
layout (location = 1) in vec3 aNormal;   // normalna ściany

uniform mat4 uModel;        // model -> świat (przesunięcie, obrót, skala obiektu)
uniform mat4 uView;         // świat -> kamera (glm::lookAt)
uniform mat4 uProjection;   // kamera -> ekran (glm::perspective)

out vec3 vNormal;

void main()
{
    // Macierz normalnych: poprawnie obraca normalne także przy nierównomiernej skali modelu.
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;

    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
