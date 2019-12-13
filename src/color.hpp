#ifndef COLOR_HPP_
#define COLOR_HPP_

#include "extension.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>

namespace juicer {

class Color {
private:
    glm::vec4 data_;
public:
    Color(shader);
    Color(const glm::vec4& color);
    Color(const glm::vec3& color);
    Color(GLuint color);

    GLfloat red();
    GLfloat green();
    GLfloat blue();

    void red(GLfloat r);
    void green(GLfloat g);
    void blue(GLfloat b);
    void vec(const glm::vec4& color);
    void vec(const glm::vec3& color);
    void hex(GLuint color);

    const GLfloat * data();
};

}

#endif
