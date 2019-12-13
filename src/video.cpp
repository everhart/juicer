#include "video.hpp"

#include <drm/drm_fourcc.h>
#include <EGL/eglext.h>
#include "extension.hpp"

namespace s2d {

void Video::get_egl_display_() {
    egl_display_ = eglGetCurrentDisplay();
    if (egl_display_ == EGL_NO_DISPLAY) {
        throw std::runtime_error("no egl display is available");
    }
}

//! creates a new egl image using Video::data_;
void Video::create_egl_image_() {
    // TODO: check if extension exists
    // TODO: check if the platform uses intel graphics

    static uint64_t modifier = I915_FORMAT_MOD_Y_TILED;
    EGLAttrib egl_attributes[] = {
        EGL_WIDTH, width_,
        EGL_HEIGHT, height_,
        EGL_LINUX_DRM_FOURCC_EXT, DRM_FORMAT_NV12,
        EGL_DMA_BUF_PLANE0_FD_EXT, fd_,
        EGL_DMA_BUF_PLANE0_PITCH_EXT, width_,
        EGL_DMA_BUF_PLANE0_OFFSET_EXT, 0,
        EGL_DMA_BUF_PLANE1_FD_EXT, fd_,
        EGL_DMA_BUF_PLANE1_PITCH_EXT, width_,
        EGL_DMA_BUF_PLANE1_OFFSET_EXT, width_ * height_,
        EGL_DMA_BUF_PLANE0_MODIFIER_LO_EXT, (uint32_t)modifier,
        EGL_DMA_BUF_PLANE0_MODIFIER_HI_EXT, (uint32_t)(modifier >> 32),
        EGL_DMA_BUF_PLANE1_MODIFIER_LO_EXT, (uint32_t)modifier,
        EGL_DMA_BUF_PLANE1_MODIFIER_HI_EXT, (uint32_t)(modifier >> 32),
        EGL_NONE
    };

    egl_image_ = eglCreateImage(
        egl_display_, 
        EGL_NO_CONTEXT, 
        EGL_LINUX_DMA_BUF_EXT,
        (EGLClientBuffer)NULL,
        egl_attributes
    );
    if (egl_image_ == EGL_NO_IMAGE) {
        throw std::runtime_error("could not create egl image");
    }
}

void Video::destroy_egl_image_() {
    eglDestroyImage(egl_display_, egl_image_);
}

void Video::create_texture_() {
    create_egl_image_();

    activate();
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture_);
    bind();
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_RED,
        width_, 
        height_, 
        0,
        GL_RED, 
        GL_UNSIGNED_BYTE, 
        data_.data()
    );
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEGLImageTargetTexture2DOES(EGL_TEXTURE_2D, egl_image_);

    unbind();
}

Video::Video() {
    width_ = 0;
    height_ = 0;
}

Video::Video(const std::string& filename) {
    size_t size;

    const size_t n = 4 * 1920 * 1080;
    char s[n];

    buffer_.open(filename.c_str(), std::ios_base::in);

    width_ = buffer_.getw();
    height_ = buffer_.geth();
    data_.resize(4 * width_ * height_);

    while (buffer_.sgetf(data_.data(), data_.size()) == 0);

    fd_ = buffer_.snextfd();
    get_egl_display_();
    create_egl_image_();
    create_texture_();
    destroy_egl_image_();
}

}
