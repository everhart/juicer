#ifndef PROJECTION_HPP_
#define PROJECTION_HPP_

#include "transform.hpp"

namespace juicer {
namespace transform {

class Projection : public Transform {
public:
    Projection(const Shader& shader);

    void orthographic(
        GLfloat left, GLfloat right, GLfloat bottom, GLfloat top
    );
    void perspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);
};

}
}

#endif
