#version 330 core

layout (location = 0) in vec3 aPos;     // pozycja wierzchołka z stb_easy_font (piksele, początek w lewym górnym rogu)

uniform mat4  uProjection;              // ortograficzna: (0,0) = lewy górny róg okna
uniform vec2  uOrigin;                  // gdzie w oknie narysować tekst
uniform float uScale;                   // skalowanie tekstu

void main()
{
    gl_Position = uProjection * vec4(uOrigin + aPos.xy * uScale, 0.0, 1.0);
}
