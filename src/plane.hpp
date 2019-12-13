#ifndef PLANE_HPP_
#define PLANE_HPP_

#include "shader.hpp"
#include "vertex-array.hpp"
// #include "texture.hpp"
#include "transform/model.hpp"

namespace juicer {

class Plane {
protected:
    Shader shader_;
    VertexArray vertex_array_;
    transform::Model model_;

    Plane(const Shader& shader);
    Plane(const Shader& shader, const glm::vec3& position);
public:
    Plane(const glm::vec3& position);
    virtual void draw();

    transform::Model& transform();
};
};

}

#endif
