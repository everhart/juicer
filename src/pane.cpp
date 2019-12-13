#include "pane.hpp"
#include "juicer.hpp"

namespace juicer {

Pane::Pane(const Shader& shader) : shader_(shader), model_(shader) {}

Pane::Pane(const Shader& shader, const glm::vec3& position) :
    Pane(shader)
{
    model_.position(position);
}

Pane::Pane(const glm::vec3& position) : 
    Pane(shader::pane(), position) 
{
    GLfloat w = 200.0f;
    GLfloat h = 200.0f;
    GLfloat z = position.z;
    std::vector<GLfloat> xyz = {
        0.0f, 0.0f, z,
        w, 0.0f, z,
        0.0f, h, z,
        w, h, z,
        w, 0.0f, z,
        0.0f, h, z
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

void Pane::draw() {
    shader_.use();
    vertex_array_.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_array_[0].size());
    VertexArray::unbind();
}

transform::Model& Pane::transform() {
    return model_;
}

}
