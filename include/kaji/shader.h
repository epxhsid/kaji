#pragma once

#include <GL/glew.h>
#include <string>

namespace kaji {

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

private:
    GLuint compile_shader(GLenum type, const std::string& source);
    std::string read_file(const std::string& path);

    GLuint program_id_= 0;
};

}
