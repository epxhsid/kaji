cmake_minimum_required(VERSION 3.25)
project(kaji
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(kaji
    src/application.cpp
    src/input.cpp
    src/main.cpp
    src/gfx/blend_state.cpp
    src/gfx/buffer.cpp
    src/gfx/framebuffer.cpp
    src/gfx/mesh.cpp
    src/gfx/rect.cpp
    src/gfx/shader.cpp
    src/gfx/texture.cpp
    src/math/ortographic_camera.cpp
    src/math/projection.cpp
)

#[[
    cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DKAJI_ENABLE_SANITIZERS=ON

    cmake --build build -j$(nproc)

    ASAN_OPTIONS=detect_leaks=1:halt_on_error=1:check_initialization_order=1 \
    ./build/kaji
]]
option(KAJI_ENABLE_SANITIZERS "enable ASan and LSan" OFF)
if (KAJI_ENABLE_SANITIZERS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(kaji PRIVATE
            -fsanitize=address
            -fno-omit-frame-pointer
            -g
        )

        target_link_options(kaji PRIVATE
            -fsanitize=address
        )
    else()
        message(WARNING "sanitizers are only supported on GCC and Clang")
    endif()
endif()

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
    glm::glm
)
