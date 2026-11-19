#pragma once

#include <GL/glew.h>
#include <array>
#include <string>
#include <string_view>
#include <unordered_map>

namespace kaji::gfx {

class Shader {
public:
    Shader(const std::string& vertex_path, const std::string& fragment_path);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator = (const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator = (Shader&& other) noexcept;

    void bind() const;
    void unbind() const;
    GLuint id() const;

    void set_int(std::string_view name, int value);
    void set_float(std::string_view name, float value);
    void set_vec2(std::string_view name, float x, float y);
    void set_vec4(std::string_view name, float r, float g, float b, float a);
    void set_mat4_col_major(std::string_view name, const std::array<float, 16>& matrix);

private:
    GLint uniform_location(std::string_view name);
    GLuint compile_shader(GLenum type, const std::string& source);
    std::string read_file(const std::string& path);

    GLuint program_id_= 0;
    std::unordered_map<std::string, GLint> location_cache_;
};

}
