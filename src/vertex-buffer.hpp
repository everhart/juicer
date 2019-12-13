#ifndef VERTEX_BUFFER_HPP_
#define VERTEX_BUFFER_HPP_

#include "extension.hpp"
#include "buffer.hpp"

#include <vector>

namespace juicer {

class VertexBuffer : public Buffer<GLfloat> {
    GLuint vbo_;
    void create_vbo_();

public:
    VertexBuffer(const std::vector<GLfloat>& data, GLsizei stride);

    GLuint bo() const noexcept;
    void bind() const noexcept;
    static void unbind();
};

}

#endif
