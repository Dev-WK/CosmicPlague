cmake_minimum_required(VERSION 3.16)
project(CosmicPlague VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Domyślnie budujemy w Debug (dla generatorów jednokonfiguracyjnych: Make, Ninja).
# Wersja szybka: cmake -B build -DCMAKE_BUILD_TYPE=Release
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Typ buildu" FORCE)
endif()

# ---------------------------------------------------------------------------
# Zależności pobierane automatycznie z GitHuba przy pierwszej konfiguracji
# (nie trzeba nic instalować ręcznie poza kompilatorem i CMake).
# ---------------------------------------------------------------------------
include(FetchContent)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(glfw glm)

# GLAD (OpenGL 3.3 Core) i stb są wrzucone do repozytorium w katalogu external/.
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad SYSTEM PUBLIC external/glad/include)
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})

add_library(stb INTERFACE)
target_include_directories(stb SYSTEM INTERFACE external/stb)

# ---------------------------------------------------------------------------
# Gra
# ---------------------------------------------------------------------------
# Nowe pliki .cpp w src/ są wykrywane automatycznie (CONFIGURE_DEPENDS),
# więc dodanie pliku nie wymaga edycji tego CMakeLists.txt (mniej konfliktów w gitcie).
file(GLOB_RECURSE CP_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(CosmicPlague ${CP_SOURCES})

target_include_directories(CosmicPlague PRIVATE src)
target_link_libraries(CosmicPlague PRIVATE glfw glad glm::glm stb)

target_compile_definitions(CosmicPlague PRIVATE
    GLFW_INCLUDE_NONE                                   # GL ładuje wyłącznie GLAD
    CP_ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets"         # ścieżka do shaderów (w przyszłości modeli, tekstur)
)

if(MSVC)
    target_compile_options(CosmicPlague PRIVATE /W4 /utf-8)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CosmicPlague)
else()
    target_compile_options(CosmicPlague PRIVATE -Wall -Wextra -Wpedantic)
endif()
