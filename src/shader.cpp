#include "kaji/shader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
namespace kaji {

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

Shader::Shader(Shader&& other) noexcept : program_id_(other.program_id_) {
    other.program_id_ = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (program_id_ != 0) {
            glDeleteProgram(program_id_);
        }
        program_id_ = other.program_id_;
        other.program_id_ = 0;
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
