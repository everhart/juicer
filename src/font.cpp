#include "font.hpp"

#include <stdexcept>
#include <iostream>

namespace s2d {

Font::Font(const std::string& filename, GLuint resolution) :
    filename_(filename), resolution_(resolution) {
    create_library_();
    create_face_();
    query_dimensions_();
    create_alphabet_();
    create_texture_();
}

Font::~Font() {
    destroy_face_();
    destroy_library_();
}

void Font::create_library_() {
    if (FT_Init_FreeType(&ft_)) {
        throw std::runtime_error(
            "ERROR::FREETYPE: Failed to initialize FreeType Library"
        );
    }
}

void Font::create_face_() {
    if (FT_New_Face(ft_, filename_.c_str(), 0, &face_)) {
        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
    }
    FT_Set_Pixel_Sizes(face_, 0, resolution_);    
}

void Font::query_dimensions_() {
    for (int i = 0; i < 128; i++) {
        if (FT_Load_Char(face_, i, FT_LOAD_RENDER)) {
            throw std::runtime_error("ERROR::FREETYPE: Error loading font");
        }
        auto& glyph = face_->glyph;

        width_ += glyph->bitmap.width;
        height_ = std::max(height_, glyph->bitmap.rows);
    }
}

void Font::create_alphabet_() {
    int x = 0;
    for (int i = 0; i < 128; i++) {
        if (FT_Load_Char(face_, i, FT_LOAD_RENDER)) {
            throw std::runtime_error(
                "ERROR::FREETYPE: Failed to load character"
            );
        }
        auto& glyph = face_->glyph;

        alphabet_[i].advance = glm::vec2(
            glyph->advance.x >> 6, glyph->advance.y >> 6
        );
        alphabet_[i].position = glm::vec2(
            glyph->bitmap_left, glyph->bitmap_top
        );
        alphabet_[i].width = glyph->bitmap.width;
        alphabet_[i].height = glyph->bitmap.rows;
        alphabet_[i].offset = (float)x / width_;    

        x += glyph->bitmap.width;
    }
}

void Font::create_texture_() {
    glActiveTexture(GL_TEXTURE0 + id_);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_RED, 
        width_, 
        height_, 
        0,
        GL_RED,
        GL_UNSIGNED_BYTE, 
        NULL
    );
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int x = 0;
    for (int i = 0; i < 128; i++) {
        if (FT_Load_Char(face_, i, FT_LOAD_RENDER)) {
            throw std::runtime_error(
                "ERROR::FREETYPE: Failed to load character"
            );
        }
        auto& glyph = face_->glyph;

        glTexSubImage2D(
            GL_TEXTURE_2D, 
            0, 
            x, 
            0, 
            glyph->bitmap.width, 
            glyph->bitmap.rows,
            GL_RED, 
            GL_UNSIGNED_BYTE,
            glyph->bitmap.buffer
        );
        x += glyph->bitmap.width;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Font::destroy_face_() {
    FT_Done_Face(face_);
}

void Font::destroy_library_() {
    FT_Done_FreeType(ft_);
}

const Font::Glyph * Font::alphabet() const noexcept {
    return alphabet_;
}

}
