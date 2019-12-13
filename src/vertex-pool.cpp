#include "vertex-pool.hpp"

#include <cstring>

namespace juicer {

VertexPool::Block::Block(GLsizei index) {
}

Block VertexPool::request_block_()
{
    // if no fragments are available
    if (fragment_count_ == 0) {
        GLsizei index = count();
        data_.resize(data_.size() + Block::size);

        return Block(index);
    }
    
    Block block = Block(fragment_head_);
    fragment_head_ = block.next();

    return block;
}

void VertexPool::return_block_(GLsizei index)
{
    if (fragment_count_ == 0) {
        fragments_head_ = index;
        return;
    }

    Block block = Block(index);
    block.next(fragments_head_);
    fragments_head_ = index;
}

VertexPool::VertexPool(size_t size) {
    data_ = std::vector<Block>(size);
    fragments_head_ = 0;
    fragment_count_ = 0;
}

size_t VertexPool::push(
    glm::vec4& color, glm::mat4& transform
) {
    size_t index;
    Block block;
    GLubyte * data = (GLubyte *)block.data;

    // TODO: don't use memcpy
    memcpy(data, &color[0], 16);
    memcpy(data + 16, &transform[0], 64);

    if (fragment_count_ == 0) {
        index = data_.size();

        *(GLuint *)(data + 70) = 1;
        data_.push_back(block);
    }
    else {
        index = fragments_head_;

        // pop the front of fragments list
        Block * fragment = &data_[index]; 
        fragments_head_ = fragment->next;

        // set the popped fragment
        *(GLuint *)(data + 70) = 0;
        *fragment = block;
        --fragment_count_;
    }

    return index;
}

// when we create geometry we will return it's index
void VertexPool::remove(size_t index) {
    Block * block = &data_[index];
    Block * head = &data_[fragments_head_];
    GLubyte * data = block->data;

    // set mask
    *(GLfloat *)(data + 70) = 0.0f;

    // push to the front of fragments list
    if (fragment_count_ > 0) {
        block->next = head->next;
    }

    fragments_head_ = index;
    ++fragment_count_;
}

void VertexPool::resize(size_t size) {
    return data_.resize(size);
}

GLsizei VertexPool::size() {
    return data_.size();
}

}
