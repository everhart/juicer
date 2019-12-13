#ifndef ELEMENT_BUFFER_HPP_
#define ELEMENT_BUFFER_HPP_

#include "extension.hpp"
#include "buffer.hpp"

#include <vector>

namespace juicer {

class ElementBuffer : public Buffer<GLfloat> {
    GLuint ebo_;
    void create_ebo_();

public:
    ElementBuffer(const std::vector<GLfloat>& data, GLsizei stride);

    GLuint bo() const noexcept;
    void bind() const noexcept;
    static void unbind();
};

}

#endif
