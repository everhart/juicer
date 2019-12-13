#include "view.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace juicer {
namespace transform {

View::View(
    const Shader& shader, glm::vec3 eye, glm::vec3 center, glm::vec3 up
) : Transform(shader),
    eye_(eye),
    center_(center),
    up_(up)
{
    create_uniform_("view");
    matrix(glm::lookAt(eye_, center_, up_));
}

void View::eye(glm::vec3 pos) {
    eye_ = pos;
    matrix(glm::lookAt(eye_, center_, up_));
}

void View::center(glm::vec3 pos) {
    center_ = pos;
    matrix(glm::lookAt(eye_, center_, up_));
}

void View::up(glm::vec3 axis) {
    up_ = axis;
    matrix(glm::lookAt(eye_, center_, up_));
}

}
}
