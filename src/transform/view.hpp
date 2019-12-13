#ifndef VIEW_HPP_
#define VIEW_HPP_

#include "transform.hpp"

namespace juicer {
namespace transform {

class View : public Transform {
private:
    glm::vec3 eye_;
    glm::vec3 center_;
    glm::vec3 up_;

public:
    View(const Shader& shader, glm::vec3 eye, glm::vec3 center, glm::vec3 up);

    void eye(glm::vec3 pos);
    void center(glm::vec3 pos);
    void up(glm::vec3 axis);
};

}
}

#endif
