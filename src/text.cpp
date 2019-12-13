#include "text.hpp"

#include "s2d.hpp"

namespace s2d {

Text::Text(
    const Font& font,
    const glm::vec2& position,
    const std::string& message,
    GLfloat z
) : Media(s2d::shader::text(), position, z), message_(message) {
    texture_ = font;
    create_sampler_();
    create_attribute_(font);
}

void Text::create_attribute_(const Font& font) {
    auto xyz = std::vector<GLfloat>();
    auto uv = std::vector<GLfloat>();
    xyz.reserve(18 * message_.size());
    uv.reserve(12 * message_.size());

    int x = 0.0f;
    int y = 0.0f;
    GLfloat size = 1.0f;

    for (auto c : message_) {
        auto& glyph = font.alphabet()[c];

        GLfloat n =  x + glyph.position.x * size;              // normalized x
        GLfloat m = -y - glyph.position.y * size;              // normalized y
        GLfloat u = glyph.offset + glyph.width / font.width(); // texture x
        GLfloat v = glyph.height / font.height();              // texture y

        x += glyph.advance.x * size;
        y += glyph.advance.y * size;

        if (!glyph.width || !glyph.height) continue;

        GLfloat xyz_temp[18] = {
            n,               -m,                0.0f,
            n,               -m - glyph.height, 0.0f,
            n + glyph.width, -m,                0.0f,
            n + glyph.width, -m,                0.0f,
            n,               -m - glyph.height, 0.0f,
            n + glyph.width, -m - glyph.height, 0.0f
        };

        GLfloat uv_temp[12] = {
            glyph.offset, 0,
            glyph.offset, v, 
            u,            0,
            u,            0,
            glyph.offset, v,
            u,            v           
        };

        for (int i = 0; i < 18; i++) {
            xyz.push_back(xyz_temp[i]);
        }
        for (int i = 0; i < 12; i++) {
            uv.push_back(uv_temp[i]);
        }
    }

    attribute_ = Attribute({ Buffer(xyz, 3), Buffer(uv, 2) });
}

void Text::draw() {
    shader_.use();
    texture_.activate();
    texture_.bind();
    attribute_.bind();
    glDrawArrays(GL_TRIANGLES, 0, attribute_[0].size());
    Attribute::unbind();
    texture_.unbind();
}

}
