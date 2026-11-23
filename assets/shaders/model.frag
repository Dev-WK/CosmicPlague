#version 330 core

// Proste oświetlenie: światło otoczenia (ambient) + rozproszone (diffuse).
// Im bardziej ściana jest zwrócona do światła, tym jaśniejsza.
// (W kolejnych etapach można dodać odbłysk Phonga i mgłę.)

in vec3 vNormal;

out vec4 FragColor;

uniform vec3 uColor;      // kolor obiektu
uniform vec3 uLightDir;   // kierunek DO światła (światło kierunkowe)

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(uLightDir);

    float ambient = 0.35;
    float diffuse = max(dot(N, L), 0.0);

    FragColor = vec4(uColor * (ambient + 0.65 * diffuse), 1.0);
}
