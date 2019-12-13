#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <vector>
#include <string>
#include <iostream>

#include "media.hpp"
#include "font.hpp"

namespace s2d {

class Text : public Media {
protected:
    std::string message_;
    
    void create_attribute_(const Font& font);
public:
    Text(
        const Font& font,
        const glm::vec2& position,
        const std::string& message,
        GLfloat z
    );
    virtual void draw() override;

    const Font& font() const noexcept;
    const std::string& message() const noexcept;
    void font(const Font& font);
    void message(const std::string& message);
};

}

#endif
