#ifndef VERTEX_ARRAY_HPP_
#define VERTEX_ARRAY_HPP_

#include "extension.hpp"
#include "buffer.hpp"
#include "vertex-buffer.hpp"
#include "vertex-attribute.hpp"

#include <vector>

namespace juicer { 

class VertexArray {
protected:
    GLuint vao_;
    GLsizei size_;

    void create_vao_();
public:
    VertexArray();
    template <class T> void push(VertexAttribute<T>& vertex_attribute);
    GLuint vao();
    void bind();
    static void unbind();
};

}

#include "vertex-array.tpp"

#endif
