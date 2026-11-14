# Cosmic Plague

Trójwymiarowa gra zręcznościowa typu fixed rail-shooter (C++17, OpenGL 3.3 Core, GLFW, GLAD, GLM).
Projekt na zajęcia z przedmiotu *Grafika 3D i Programowanie Kart Graficznych II*.

Stan obecny: pusty szablon - otwiera okno OpenGL i zamyka je klawiszem Esc.

## Budowanie

Wymagane: kompilator C++17, CMake >= 3.16, Git i karta graficzna z OpenGL 3.3.
GLFW i GLM CMake pobiera sam (FetchContent), więc przy pierwszej konfiguracji potrzebny jest internet.
GLAD i stb są w katalogu `external/`.

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/CosmicPlague        # Windows: build\Release\CosmicPlague.exe
```

Nowe pliki `.cpp` w `src/` są wykrywane automatycznie - nie trzeba edytować `CMakeLists.txt`.
