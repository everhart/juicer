#include "element-buffer.hpp"

namespace juicer {

ElementBuffer::ElementBuffer(
    const std::vector<GLfloat>& data, GLsizei stride
) : Buffer<GLfloat>(data, stride, GL_UNSIGNED_INT)
{
    create_ebo_();
}

void ElementBuffer::create_ebo_() {
    glGenBuffers(1, &ebo_);
    bind();
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        data_.size() * sizeof(GLuint),
        data_.data(),
        GL_STATIC_DRAW
    );
    unbind();
}

GLuint ElementBuffer::bo() const noexcept {
    return ebo_;
}

void ElementBuffer::bind() const noexcept {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);   
}

void ElementBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
