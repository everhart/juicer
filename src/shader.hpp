#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <string>
#include <GL/glew.h>

namespace juicer {

class Shader {
    std::string vsh_filename_;
    std::string fsh_filename_;
    GLuint vsh_;
    GLuint fsh_;
    GLuint program_;

    void load_shader_(
        std::string filename, GLuint& shader, GLenum shader_type
    );
    void create_vsh_();
    void create_fsh_();
    void create_program_();
public:
    Shader();
    Shader(const std::string& vsh_filename, const std::string& fsh_filename);

    GLuint program() const noexcept;
    void use() const noexcept;
};

}

#endif
