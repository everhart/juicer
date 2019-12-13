#include "vertex-buffer.hpp"

namespace juicer {

VertexBuffer::VertexBuffer(
    const std::vector<GLfloat>& data, GLsizei stride
) : Buffer<GLfloat>(data, stride, GL_FLOAT)
{
    create_vbo_();
}

void VertexBuffer::create_vbo_() {
    glGenBuffers(1, &vbo_);
    bind();
    glBufferData(
        GL_ARRAY_BUFFER,
        data_.size() * sizeof(GLfloat),
        data_.data(),
        GL_STATIC_DRAW
    );
    unbind();
}

GLuint VertexBuffer::bo() const noexcept {
    return vbo_;
}

void VertexBuffer::bind() const noexcept {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);   
}

void VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
