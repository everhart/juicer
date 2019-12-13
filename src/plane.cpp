#include "plane.cpp"

namespace juicer {

Plane::Plane(const Shader& shader) : shader_(shader), model_(shader) {}

Plane::Plane(const Shader& shader, const glm::vec3& position) :
    Plane(shader)
{
    model_.position(position);
}

Plane::Plane(glm::vec3& position) : 
    Plane(shader::plane(), position) 
{
    GLfloat w = 200.0f;
    GLfloat h = 200.0f;
    GLfloat z = position.z;

    // xyz coordinates
    std::vector<GLfloat> xyz = {
        0.0f, 0.0f, 0.0f,
        w, 0.0f, 0.0f,
        0.0f, 0.0f, h,
        w, 0.0f, h,
        w, 0.0f, 0.0f,
        0.0f, 0.0f, h
    };
  
    std::vector<GLfloat> uv = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f
    };
    vertex_array_ = VertexArray({ VertexBuffer(xyz, 3), VertexBuffer(uv, 2) });
}

void Plane::draw() {
    shader_.use();
    vertex_array_.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_array_[0].size());
    VertexArray::unbind();
}

transform::Model& Plane::transform() {
    return model_;
}

}
