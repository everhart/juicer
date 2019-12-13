#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "transform.hpp"

namespace juicer {
namespace transform {

class Model : public Transform {
    glm::mat4 translation_;
    glm::mat4 rotation_;
    glm::mat4 scale_;
public:
    Model(const Shader& shader);

    void position(GLfloat x, GLfloat y, GLfloat z);
    void position(const glm::vec3& pos);
    void translate(GLfloat x, GLfloat y, GLfloat z);
    void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
    void scale(GLfloat x, GLfloat y, GLfloat z);
    void scale(GLfloat s);
};

}
}

#endif
