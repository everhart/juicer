#ifndef POOL_HPP_
#define POOL_HPP_

#include "extension.hpp"

#include <vector>
#include <glm/glm.hpp>

namespace juicer {

// TODO: change it to be a chunk based system
class VertexPool {
private:
    class Block {
    private:
        GLsizei index_;

    public:
        static const size;

        Block(GLsizei index);
        GLsizei index();

        GLsizei next();
        void  next(GLsizei index);

        void color(glm::vec4& vec);
        void transform(glm::mat4& model);
        void mask(GLfloat mask);
    };

    std::vector<GLfloat> buffer_;   // buffer containing all blocks

    GLsizei fragments_head_;        // index of head fragment block
    GLsizei fragment_count_;        // number of fragment blocks

    Block request_block_();
    void return_block_(Block& block);
public:
    VertexPool(GLsizei count);

    //! pushes a new vertex to the buffer back of the buffer
    size_t push(glm::vec4& color, glm::mat4& transform);
    //! makes the vertex at the specified index a fragment
    void remove(GLsizei index);

    //! returns a pointer to the buffer
    GLfloat * data();
    //! returns the size, in bytes, of the buffer
    GLsizei size();
    //! returns the number of vertices composing the buffer
    GLsizei count();
};

}

#endif
