#include "media.hpp"

#include <iostream>

#include <png.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "s2d.hpp"

namespace s2d {

Media::Media(const Shader& shader, const glm::vec2& position, GLfloat z) :
    Graphic(shader, position, z) {
}

Media::Media(
    const glm::vec4& color,
    const Image& image, 
    const glm::vec2& position,
    GLfloat z
) : Graphic(s2d::shader::media(), position, z) {
    GLfloat w = image.width();
    GLfloat h = image.height();
    std::vector<GLfloat> xyz = {
        0.0f, 0.0f, z,
          w,  0.0f, z,
        0.0f,    h, z,
           w,    h, z,
           w, 0.0f, z,
        0.0f,    h, z
    };
   
    std::vector<GLfloat> uv = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f
    };  

    texture_ = image;
    create_sampler_();

    attribute_ = Attribute({ Buffer(xyz, 3), Buffer(uv, 2) });
}

void Media::create_sampler_() {
    sampler_ = glGetUniformLocation(shader_.program(), "sampler");
    shader_.use();
    glUniform1i(sampler_, texture_.id());   
}

void Media::draw() {
    shader_.use();
    texture_.activate();
    texture_.bind();
    Graphic::draw();
    Texture::unbind();
}

}
