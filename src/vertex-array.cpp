#include "vertex-array.hpp"

#include <stdexcept>

namespace juicer {

VertexArray::VertexArray() : size_(0) {
    create_vao_();
}

void VertexArray::create_vao_() {
    glGenVertexArrays(1, &vao_);
    bind();
}

GLuint VertexArray::vao() {
    return vao_;
}

void VertexArray::bind() {
    glBindVertexArray(vao_);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}

}
