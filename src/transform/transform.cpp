#include "transform.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace juicer {
namespace transform {

Transform::Transform(const Shader& shader) : shader_(shader) {
    matrix_ = glm::mat4();
}

Transform::Transform(const Shader& shader, const glm::mat4& m) : 
    shader_(shader), matrix_(m)
{
    create_uniform_("transform");
}

void Transform::create_uniform_(const std::string& name) {
    uniform_ = glGetUniformLocation(shader_.program(), name.c_str());
    shader_.use();
    glUniformMatrix4fv(uniform_, 1, GL_FALSE, data());   
}

GLuint Transform::uniform() const noexcept {
    return uniform_;
}

void Transform::matrix(const glm::mat4& m) {
    matrix_ = m;
    glUniformMatrix4fv(uniform_, 1, GL_FALSE, data());
}

const glm::mat4& Transform::matrix() {
    return matrix_;
}

const GLfloat * Transform::data() {
    return glm::value_ptr(matrix_);
}

}
}
