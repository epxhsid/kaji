#include "kaji/gfx/shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
namespace kaji::gfx {

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path) {
    std::string vertex_source = read_file(vertex_path);
    std::string fragment_source = read_file(fragment_path);
    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_source);
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_source);

    program_id_ = glCreateProgram();
    glAttachShader(program_id_, vertex_shader);
    glAttachShader(program_id_, fragment_shader);
    glLinkProgram(program_id_);

    GLint success = GL_FALSE;
    glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
    if (!success) {
        GLint length = 0;
        glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, '\0');
        glGetProgramInfoLog(program_id_, length, nullptr, log.data());
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        throw std::runtime_error("Shader linking error:\n" + log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader() {
    if (program_id_ != 0) {
        glDeleteProgram(program_id_);
    }
}

Shader::Shader(Shader&& other) noexcept : program_id_(other.program_id_), location_cache_(std::move(other.location_cache_)) {
    other.program_id_ = 0;
    other.location_cache_.clear();
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (program_id_ != 0) {
            glDeleteProgram(program_id_);
        }

        program_id_ = other.program_id_;
        location_cache_ = std::move(other.location_cache_);

        other.program_id_ = 0;
        other.location_cache_.clear();
    }
    return *this;
}

void Shader::bind() const {
    glUseProgram(program_id_);
}

void Shader::unbind() const {
    glUseProgram(0);
}

GLuint Shader::id() const {
    return program_id_;
}

void Shader::set_int(std::string_view name, int value) {
    const GLint loc = uniform_location(name);
    if (loc != -1) {
        glUniform1i(loc, value);
    }
}

void Shader::set_float(std::string_view name, float value) {
    const GLint loc = uniform_location(name);
    if (loc != -1) {
        glUniform1f(loc, value);
    }
}

void Shader::set_vec2(std::string_view name, float x, float y) {
    const GLint loc = uniform_location(name);
    if (loc != -1) {
        glUniform2f(loc, x, y);
    }
}

void Shader::set_vec4(std::string_view name, float r, float g, float b, float a) {
    const GLint loc = uniform_location(name);
    if (loc != -1) {
        glUniform4f(loc, r, g, b, a);
    }
}

void Shader::set_mat4_col_major(std::string_view name, const std::array<float, 16>& matrix) {
    const GLint loc = uniform_location(name);
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, matrix.data());
    }
}

GLint Shader::uniform_location(std::string_view name) {
    // std::unordered_map<std::string, ...> needs a null-terminated key for glGetUniformLocation
    std::string key(name);
    if (const auto it = location_cache_.find(key); it != location_cache_.end()) {
        return it->second;
    }

    const GLint loc = glGetUniformLocation(program_id_, key.c_str());
    location_cache_.emplace(key, loc);

    #ifndef NDEBUG
    if (loc == -1) {
        std::cerr << "uniform '" << key << "' not found in shader program\n";
    }
    #endif

    return loc;
}

GLuint Shader::compile_shader(GLenum type, const std::string &source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, '\0');
        glGetShaderInfoLog(shader, length, nullptr, log.data());
        glDeleteShader(shader);
        throw std::runtime_error("shader compilation error:\n" + log);
    }

    return shader;
}

std::string Shader::read_file(const std::string &path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("failed to open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

}
