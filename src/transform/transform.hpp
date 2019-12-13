#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include "../shader.hpp"

#include <glm/glm.hpp>

namespace juicer {
namespace transform {

class Transform {
protected:
    const Shader& shader_;

    glm::mat4 matrix_;
    GLuint uniform_;

    void create_uniform_(const std::string& name);

    Transform(const Shader& shader);
public:
    Transform(const Shader& shader, const glm::mat4& m);

    GLuint uniform() const noexcept;
    void matrix(const glm::mat4& data);
    const glm::mat4& matrix();
    const GLfloat * data();
};

}
}

#endif
