#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "extension.hpp"
#include "shader.hpp"

#include <iostream>

namespace s2d {

class Texture {
protected:
    GLuint id_;
    static GLuint count_;

    GLuint texture_;
    GLuint width_;
    GLuint height_;

public:
    Texture();
    // TODO: add ability for Texture object to be created from raw data

    GLuint id() const noexcept;
    GLuint texture() const noexcept;
    GLuint width() const noexcept;
    GLuint height() const noexcept;

    void update();
    void activate();
    static void deactivate();
    void bind();
    static void unbind();
};

}

#endif
