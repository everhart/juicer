#ifndef BUFFER_HPP_
#define BUFFER_HPP_

#include "extension.hpp"

#include <vector>

namespace juicer {

template <class T> class Buffer {
protected:
    GLenum type_;
    std::vector<T> data_;
    GLsizei stride_;

public:
    // TODO: add move semantics to constructor
    Buffer(const std::vector<GLfloat>& data, GLsizei stride, GLenum type);

    T * data();
    const T * data() const noexcept;
    GLsizei size() const noexcept;
    GLsizei stride() const noexcept;
    GLenum type() const noexcept;;

    virtual GLuint bo() const noexcept = 0;
    virtual void bind() const noexcept = 0;
};

}

#include "buffer.tpp"

#endif
