#include "geometry-data.hpp"

#include "cube.hpp"

namespace juicer {

GeometryData * GeometryData::instance_ = nullptr;

GeometryData::GeometryData() : vertex_buffer_(data_, 32)
{
    // create geometry
}

GeometryData& GeometryData::init() 
{
    if (instance_ != nullptr) {
        instance_ = new GeometryData();
    }

    return *instance_;
}

GeometryData& GeometryData::instance() {
    return init();
}

GLfloat * GeometryData::data() {
    return instance_->data_.data();
}

GLsizei GeometryData::size() {
    return data_.size();
}

void GeometryData::resize(GLsizei size) {
    data_.resize(size);
}

}
