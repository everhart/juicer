#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "texture.hpp"

#include <vector>
#include <string>
#include <iostream>

namespace s2d {

class Image : public Texture {
    std::vector<GLubyte> data_;     // raw image data
    bool alpha_;

    void create_texture_();
    void load_png_(const std::string& filename);
public:
    Image(const std::string& filename);
    GLubyte * raw() noexcept;
};

}

#endif
