#include "shader.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

namespace juicer {

Shader::Shader() {}
Shader::Shader(
    const std::string& vsh_filename, const std::string& fsh_filename
) : vsh_filename_(vsh_filename), fsh_filename_(fsh_filename) {
    create_vsh_();
    create_fsh_();
    create_program_();
}

void Shader::load_shader_(
     std::string filename, GLuint& shader, GLenum shader_type
) {
    try {
        // read file contents
        std::ifstream file(filename);
        if (!file.good()) {
            throw std::ios_base::failure(
                "ERROR::GL: Shader file doesn't exist"
            );
        }
        std::ostringstream oss;
        oss << file.rdbuf();

        // compile shader
        auto shader_str = oss.str();
        auto shader_c_str = shader_str.c_str();
        shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, &shader_c_str, NULL);
        glCompileShader(shader);

        GLint res = -1;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
        if (res != GL_TRUE) {
            throw std::runtime_error("ERROR::GL: Compiling shader failed");
        }
    }
    catch (std::ios_base::failure& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        GLint size = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
        auto error_log = std::vector<GLchar>(size);
        glGetShaderInfoLog(shader, size, &size, error_log.data());
        for (auto ch : error_log) {
            std::cerr << ch;
        }
    }
}

void Shader::create_vsh_() {
    load_shader_(vsh_filename_, vsh_, GL_VERTEX_SHADER);
}

void Shader::create_fsh_() {
    load_shader_(fsh_filename_, fsh_, GL_FRAGMENT_SHADER);
}

void Shader::create_program_() {
    program_ = glCreateProgram();
    glAttachShader(program_, vsh_);
    glAttachShader(program_, fsh_);
    glLinkProgram(program_);

    GLint res = -1;
    glGetProgramiv(program_, GL_LINK_STATUS, &res);
    if (res != GL_TRUE) {
        throw std::runtime_error("ERROR::GL: Linking shader program failed");
    }
}

GLuint Shader::program() const noexcept {
    return program_;
}

void Shader::use() const noexcept {
    glUseProgram(program_);
}

}
