#ifndef PRIMITIVE_DATA_HPP_
#define PRIMITIVE_DATA_HPP_

#include "extension.hpp"
#include "vertex-buffer.hpp"
#include "element-buffer.hpp"
#include "vertex-pool.hpp"

namespace juicer {

class PrimitiveData {
private:
    static PrimitiveData * instance_;

    VertexPool      vertex_pool_;
    VertexBuffer    vertex_buffer_;
    ElementBuffer   element_buffer_;
public:
    PrimitiveData();
    static PrimitiveData& instance();

    GLuint vbo();
    GLuint ebo();
    // friend Primitive;
};

}

#endif
