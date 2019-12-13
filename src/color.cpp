#include "color.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace juicer {

void Color::create_uniform_() {
    uniform_ = glGetUniformLocation(shader_.program(), "color");
    update_uniform_();
}

void Color::update_uniform_() {
    shader_.use();
    glUniform4fv(uniform_, 1, data());
}

Color::Color(const Shader& shader) : 
    shader_(shader), data_(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) 
{
    create_uniform_();
}

Color::Color(const Shader& shader, const glm::vec4& color) : 
    shader_(shader), data_(color) 
{
    create_uniform_();
}

void Color::vec(const glm::vec4& color) {
    data_ = color;
    update_uniform_();
}

void Color::vec(const glm::vec3& color) {
    data_ = glm::vec4(color.x, color.y, color.z, 1.0f);
    update_uniform_();
}

GLfloat Color::red() {
    return data_.x;
}

GLfloat Color::green() {
    return data_.y;
}

GLfloat Color::blue() {
    return data_.z;
}

void Color::red(GLfloat r) {
    data_.x = r;
    update_uniform_();
}

void Color::green(GLfloat g) {
    data_.y = g;
    update_uniform_();
}

void Color::blue(GLfloat b) {
    data_.z = b;
    update_uniform_();
}

const GLfloat * Color::data() {
    return glm::value_ptr(data_);
}

}
