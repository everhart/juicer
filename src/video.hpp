#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "videobuf.hpp"
#include "texture.hpp"

#include <vector>
#include <string>
#include <iostream>

namespace s2d {

class Video : public Texture {
    EGLDisplay egl_display_;
    EGLImage egl_image_;

    int fd_;
    std::string filename_;
    std::string data_;
    videobuf buffer_;

    void get_egl_display_();
    void create_egl_image_();
    void destroy_egl_image_();
    void create_texture_();

    // we need to get back to where we were

    void load_(const std::string& filename);
public:
    Video();
    Video(const std::string& filename);
    GLubyte * raw() noexcept;
};

}

#endif
