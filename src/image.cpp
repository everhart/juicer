#include "image.hpp"

#include <png.h>
#include <iostream>
#include <cstring>

namespace s2d {

Image::Image(const std::string& filename) {
    load_png_(filename);
    create_texture_();
}

void Image::load_png_(const std::string& filename) {
    FILE *  fp;
    png_structp png_ptr;
    png_infop   png_info_ptr;

    try {
        fp = fopen(filename.c_str(), "rb");
        if (fp == NULL) {
            throw std::ios_base::failure("ERROR::S2D: PNG file doesn't exist");
        }

        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (png_ptr == NULL) {
            throw std::runtime_error("ERROR::S2D: Failed to load PNG file");
        }

        png_info_ptr = png_create_info_struct(png_ptr);
        if (png_info_ptr == NULL) {
            throw std::runtime_error("ERROR::S2D: Failed to load PNG file");
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            throw std::runtime_error("ERROR::S2D: Failed to load PNG file");
        }

        unsigned int png_sig_read = 0;
        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, png_sig_read);
        png_read_png(
            png_ptr, 
            png_info_ptr, 
            PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND,
            NULL
        );

        int png_color_type = 0;
        int png_interlace_type = 0;
        int png_bit_depth = 0;
        png_uint_32 png_width = 0;
        png_uint_32 png_height = 0;

        png_get_IHDR(
            png_ptr, 
            png_info_ptr, 
            &png_width, 
            &png_height, 
            &png_bit_depth, 
            &png_color_type,
            &png_interlace_type, 
            NULL, 
            NULL
        );
        alpha_ = png_color_type & PNG_COLOR_MASK_ALPHA;

        width_ = png_width;
        height_ = png_height;

        unsigned int png_row_bytes;
        png_bytepp png_row_ptrs;

        png_row_bytes = png_get_rowbytes(png_ptr, png_info_ptr);
        data_ = std::vector<GLubyte>(png_row_bytes * png_height);
        png_row_ptrs = png_get_rows(png_ptr, png_info_ptr);

        data_.reserve(width_ * height_);
        for (int i = 0; i < png_height; i++) {
            memcpy(
                data_.data() + (png_row_bytes * (png_height - 1 - i)), 
                png_row_ptrs[i], 
                png_row_bytes
            );
        }

        png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
        fclose(fp);
    }
    catch (std::ios_base::failure& e) {
        std::cerr << e.what() << std::endl;
        fclose(fp);
    }
    catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
        fclose(fp);
    }
}

void Image::create_texture_() {
    activate();
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture_);
    bind();
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        alpha_ ? GL_RGBA : GL_RGB, 
        width_, 
        height_, 
        0,
        alpha_ ? GL_RGBA : GL_RGB, 
        GL_UNSIGNED_BYTE, 
        data_.data()
    );
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind();
}

GLubyte * Image::raw() noexcept {
    return data_.data();
}

}
