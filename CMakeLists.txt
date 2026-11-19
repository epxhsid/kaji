cmake_minimum_required(VERSION 3.25)
project(kaji
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(kaji
    src/application.cpp
    src/main.cpp
    src/mesh.cpp
    src/gfx/shader.cpp
)

target_include_directories(kaji PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

target_compile_definitions(kaji PRIVATE
    KAJI_SHADER_DIR="${PROJECT_SOURCE_DIR}/shaders"
)

target_compile_features(kaji PRIVATE cxx_std_23)
set_target_properties(kaji PROPERTIES
    CXX_EXTENSIONS OFF
)

target_link_libraries(kaji PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
)
