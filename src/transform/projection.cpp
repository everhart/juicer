#include "projection.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace juicer {
namespace transform {

Projection::Projection(const Shader& shader) : Transform(shader) {
    create_uniform_("projection");
}

void Projection::orthographic(
    GLfloat left, GLfloat right, GLfloat bottom, GLfloat top
) {
    matrix(glm::ortho(left, right, bottom, top));
}

void Projection::perspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far) {
    matrix(glm::perspective(fov, aspect, near, far));
}

}
}
