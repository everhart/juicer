#ifndef FONT_HPP_
#define FONT_HPP_

#include "extension.hpp"
#include "texture.hpp"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>

#include <string>

namespace s2d {

class Font : public Texture { 
    std::string filename_;
    GLuint resolution_;     // face size in pixels

    FT_Library ft_;
    FT_Face face_;
    
    struct Glyph {
        glm::vec2 advance;
        glm::vec2 position;
        GLfloat width;
        GLfloat height;
        GLfloat offset;
    } alphabet_[256];

    void create_library_();
    void create_face_();
    void create_alphabet_();
    void query_dimensions_();
    void create_texture_();
    void destroy_face_();
    void destroy_library_();
public:
    Font(const std::string& face, GLuint resolution);
    // Font(const Font& other);
    // void operator=(const Font& other);
    ~Font();

    const std::string& filename() const noexcept;
    GLuint size() const noexcept;
    const Glyph * alphabet() const noexcept;
};

}

#endif
