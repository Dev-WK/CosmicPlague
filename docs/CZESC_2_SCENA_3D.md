# Część 2 - Scena 3D

Gałąź: `feature/scena-3d`  |  Autor: Jakub Nicze

## Co robi ta część

Podstawy renderowania 3D: wczytanie shaderów, kamera z perspektywą, model w pamięci karty graficznej i siatka.

## Pliki

| Plik | Zawartość |
|------|-----------|
| `src/core/Shader.h/.cpp` | wczytanie kodu GLSL z plików, kompilacja, linkowanie, `setMat4` / `setVec3` itd. |
| `src/render/Camera.h/.cpp` | `view()` = `glm::lookAt`, `projection()` = `glm::perspective` |
| `src/render/Mesh.h/.cpp` | lista wierzchołków (pozycja + normalna) wysłana do VAO/VBO, `draw()` |
| `src/render/GridRenderer.h/.cpp` | linie siatki na płaszczyźnie Y = 0, rysowane jako `GL_LINES` |
| `assets/shaders/flat.vert/.frag` | shader jednokolorowych linii |

## Zależności

Ta część nie zależy od żadnej innej - można ją skompilować samą na szablonie.
Nic jeszcze nie rysuje na ekranie, dopóki część 3 (`World`) i część 1 (`Game`) jej nie użyją.

## Jak to działa

1. **Shader**: kod GLSL jest czytany z pliku, kompilowany osobno dla wierzchołków i fragmentów,
   potem łączony w program. Błędy kompilacji wyjątkiem z logiem OpenGL.
2. **Kamera**: stoi w (0; 3,5; 8), patrzy w (0; 0; -15), kąt widzenia 55 stopni, płaszczyzny odcięcia 0,1 i 150.
   `setAspect` jest wołane z proporcją okna, żeby obraz się nie rozciągał.
3. **Wierzchołek na ekranie**: `gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0)`.
   Model -> świat, widok -> układ kamery, projekcja -> perspektywa.
4. **Mesh**: VAO zapamiętuje układ danych (atrybut 0 = pozycja, atrybut 1 = normalna), VBO trzyma bajty.
5. **Siatka**: linie podłużne co 2,5 jednostki (x od -7,5 do 7,5) i poprzeczne co 2,5 (z od 10 do -60).
   Rysowana jest w rzeczywistych współrzędnych 3D, więc perspektywa sama zbliża linie do siebie na horyzoncie.

## Pytania do samodzielnego przećwiczenia

* Co robi macierz widoku, a co macierz projekcji?
* Dlaczego siatka zaczyna się dla z = 10, czyli za kamerą?
* Czym różni się VAO od VBO?
* Dlaczego linie na horyzoncie zlewają się w jedną plamę?
