#include "model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace juicer {
namespace transform {

Model::Model(const Shader& shader) : Transform(shader) {
    create_uniform_("model");

    translation_ = glm::mat4(1.0f);
    rotation_ = glm::mat4(1.0f);
    scale_ = glm::mat4(1.0f);
}

//! change the position to a to a new @x, @y, @z location
void Model::position(GLfloat x, GLfloat y, GLfloat z) {
    shader_.use();
    translation_ = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    matrix(translation_ * rotation_ * scale_);
}

//! change the position to @pos
void Model::position(const glm::vec3& pos) {
    position(pos.x, pos.y, pos.z);
}

//! translate by @x, @y, @z units
void Model::translate(GLfloat x, GLfloat y, GLfloat z) {
    shader_.use();
    translation_ *= glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    matrix(translation_ * rotation_ * scale_);
}

//! rotate about the @x, @y, @z axis
void Model::rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    // rotation_ *= glm::rotate(glm::mat4(1.0f), angle, x, y, z);
    matrix(translation_ * rotation_ * scale_);
}

//! scale by a factor of @x, @y, @z
void Model::scale(GLfloat x, GLfloat y, GLfloat z) {
    shader_.use();
    scale_ *= glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
    matrix(translation_ * rotation_ * scale_);
}

//! scale in the x, y and z direction by a factor of @s
void Model::scale(GLfloat s) {
    scale(s, s, s);
}

}
}
