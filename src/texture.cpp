#include "texture.hpp"

namespace s2d {

GLuint Texture::count_ = 0;

Texture::Texture() : width_(0), height_(0) {
    id_ = count_++;
}

GLuint Texture::id() const noexcept {
    return id_;
}

GLuint Texture::texture() const noexcept {
    return texture_;
}

GLuint Texture::width() const noexcept {
    return width_;
}

GLuint Texture::height() const noexcept {
    return height_;
}

void Texture::activate() {
    glActiveTexture(GL_TEXTURE0 + id_);
}

void Texture::deactivate() {
    glActiveTexture(GL_TEXTURE0);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, texture_);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

}
