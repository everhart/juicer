#ifndef PANE_HPP_
#define PANE_HPP_

#include "shader.hpp"
#include "vertex-array.hpp"
#include "texture.hpp"
#include "transform/model.hpp"

namespace juicer {

class Pane {
protected:
    Shader shader_;
    VertexArray vertex_array_;
    transform::Model model_;

    Pane(const Shader& shader);
    Pane(const Shader& shader, const glm::vec3& position);
public:
    Pane(const glm::vec3& position);
    virtual void draw();

    transform::Model& transform();
};

}

#endif
