#ifndef GEOMETRY_DATA_HPP_
#define GEOMETRY_DATA_HPP_

#include "vertex-buffer.hpp"
#include "cube.hpp"

namespace juicer {

class GeometryData {
private:
    static GeometryData * instance_;

    std::vector<GLfloat> data_;
    VertexBuffer vertex_buffer_;

    Cube cube_;
public:
    GeometryData();
    static GeometryData& init();
    static GeometryData& instance();

    GLfloat * data();
    GLsizei size();
    void resize(GLsizei size);
};

}

#endif
