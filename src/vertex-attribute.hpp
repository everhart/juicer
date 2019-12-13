#ifndef VERTEX_ATTRIBUTE_HPP_
#define VERTEX_ATTRIBUTE_HPP_

namespace juicer {

template <class T> class VertexAttribute {
protected:
    Buffer<T>& buffer_;
    GLsizei offset_;
    GLsizei stride_;
public:
    VertexAttribute(
        Buffer<T>& buffer, GLsizei offset, GLsizei stride
    );

    Buffer<T>& buffer();
    GLsizei offset();
    GLsizei stride();
};

}

#include "vertex-attribute.tpp"

#endif
