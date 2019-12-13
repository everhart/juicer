#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include "extension.hpp"

#include <vector>

namespace juicer {

class GeometryData;

class Geometry {
protected:
    GLsizei vertex_count_;
    GLsizei face_count_;
    GLsizei size_;

    GeometryData& geometry_data_;
    GLfloat * data_;
    GLsizei offset_;

    const std::vector<GLfloat> vertex_data_;
    std::vector<GLfloat> face_normal_data_;
    std::vector<GLfloat> vertex_normal_data_;
    const std::vector<GLfloat> uv_data_;

    void create_face_normal_data_();
    void create_vertex_normal_data_();
    void create_normal_data_();

    void interleave_vertex_data_();
    void interleave_vertex_normal_data_();
    void interleave_uv_data_();
    void interleave_data_();

    Geometry(
        const std::vector<GLfloat>& vertex_data, 
        const std::vector<GLfloat>& uv_data
    );
public:
    static const GLsizei vertex_offset;
    static const GLsizei vertex_pitch;
    static const GLsizei normal_offset;
    static const GLsizei normal_pitch;
    static const GLsizei uv_offset;
    static const GLsizei uv_pitch;
    static const GLsizei pitch;

    GLfloat * data();
    GLsizei size();
    GLsizei count();
};

}

#endif
