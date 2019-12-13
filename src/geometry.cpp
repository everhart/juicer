#include "geometry.hpp"

#include "geometry-data.hpp"

#include <glm/glm.hpp>

namespace juicer {

const GLsizei Geometry::vertex_offset = 0;

const GLsizei Geometry::vertex_pitch = 3 * sizeof(GLfloat);

const GLsizei Geometry::normal_offset = 12;

const GLsizei Geometry::normal_pitch = 3 * sizeof(GLfloat);

const GLsizei Geometry::uv_offset = 24;

const GLsizei Geometry::uv_pitch = 2  * sizeof(GLfloat);

const GLsizei Geometry::pitch = 32;

Geometry::Geometry(
    const std::vector<GLfloat>& vertex_data, 
    const std::vector<GLfloat>& uv_data
) : geometry_data_(GeometryData::instance())
{
    if ((vertex_data.size()) / 3 != (uv_data.size() / 2)) {
        // TODO: throw exception
    }
 
    // set size and count variables   
    vertex_count_ = vertex_data.size() / 3;
    face_count_ = vertex_count_ / 3;
    size_ = vertex_count_ * pitch;

    // resize geometry_data_ to accomodate the new geometry and set this new
    // geometry's datastore location
    offset_ = geometry_data_.size();
    geometry_data_.resize(offset_ + size_);
    data_ = geometry_data_.data() + offset_;

    create_normal_data_();
    interleave_data_();
}

void Geometry::interleave_vertex_data_() 
{
    // TODO: check and correct winding order
    for (int i = 0; i < vertex_count_; i++) {
        int n = i * pitch + vertex_offset;
        int m = i * 3;

        data_[n] = vertex_data_[m];
        data_[n + 1] = vertex_data_[m + 1];
        data_[n + 2] = vertex_data_[m + 2];
    }
}

void Geometry::create_face_normal_data_() {
    std::vector<GLfloat> face_normals_(face_count_);

    for (int i = 0; i < face_count_; i++) {
        int n = i * 9;
        int m = i * 3;
        
        glm::vec3 a = { vertex_data_[n], vertex_data_[n + 1], vertex_data_[n + 2] };
        glm::vec3 b = { vertex_data_[n + 3], vertex_data_[n + 4], vertex_data_[n + 5] };
        glm::vec3 c = { vertex_data_[n + 6], vertex_data_[n + 7], vertex_data_[n + 8] };

        glm::vec3 u = b - a;
        glm::vec3 v = c - a;
        glm::vec3 face_normal = cross(u, v);
        
        face_normal_data_[m] = face_normal[0];
        face_normal_data_[m + 1] = face_normal[1];
        face_normal_data_[m + 2] = face_normal[2];
    }
}

void Geometry::create_vertex_normal_data_() {
    for (int i = 0; i < face_count_; i++) {
        int n = i * 3;

        for (int j = 0; j < 3; j++) {
            int m = (i * 9) + (j * 3);

            vertex_normal_data_[m] = face_normal_data_[n + j];
            vertex_normal_data_[m + 1] = face_normal_data_[n + j];
            vertex_normal_data_[m + 2] = face_normal_data_[n + j];
        }
    }
}

void Geometry::create_normal_data_() {
    create_face_normal_data_();
    create_vertex_normal_data_();
}

void Geometry::interleave_vertex_normal_data_() {
    for (int i = 0; i < vertex_count_; i++) {
        int n = i * pitch + normal_offset;
        int m = i * 3;

        data_[n] = vertex_normal_data_[m];
        data_[n + 1] = vertex_normal_data_[m + 1];
        data_[n + 2] = vertex_normal_data_[m + 2];
    }
}

void Geometry::interleave_uv_data_() {
    for (int i = 0; i < vertex_count_; i++) {
        int n = i * pitch + uv_offset;
        int m = i * 2;

        data_[n] = vertex_normal_data_[m];
        data_[n + 1] = vertex_normal_data_[m + 1];
    }
}

void Geometry::interleave_data_() {
    interleave_vertex_data_();
    interleave_vertex_normal_data_();
    interleave_uv_data_();
}

}
