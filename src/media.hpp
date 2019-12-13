#ifndef MEDIA_HPP
#define MEDIA_HPP

#include "shader.hpp"
#include "image.hpp"
#include "graphic.hpp"

#include <vector>
#include <string>

namespace s2d {

class Media : public Graphic {
protected:
    Texture texture_;
    GLuint sampler_;

    void create_sampler_();

    Media(const Shader& shader, const glm::vec2& position, GLfloat z);
public:
    Media(
        const glm::vec4& color,
        const Image& image, 
        const glm::vec2& position,
        GLfloat z
    );

    virtual void draw() override;
};

}

#endif
