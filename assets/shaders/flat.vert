#version 330 core

// Shader jednokolorowy - używany do linii siatki.

layout (location = 0) in vec3 aPos;

uniform mat4 uModel;        // model -> świat
uniform mat4 uView;         // świat -> kamera (glm::lookAt)
uniform mat4 uProjection;   // kamera -> ekran (glm::perspective)

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
