#include <iostream>

#include "juicer.hpp"
#include "pane.hpp"

int main() {
    juicer::init(1920, 540);

    /*
    juicer::Pane pane(glm::vec3(0.0f, 0.0f, 0.0f));

    juicer::Image image("aad.png");
    juicer::Media media(
        glm::vec4(0.2f, 0.8f, 1.0f, 1.0f), image, glm::vec2(0.0f, 0.0f), 0.0f
    );
    juicer::Font font("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 128);
    juicer::Text text(font, glm::vec2(0.0f, 0.0f), "Hello World!", 0.0f);
    */

    GLfloat x = 0.0f, y = 0.0f;
    while(juicer::available()) {
        juicer::present();
        juicer::clear();
    }
    juicer::quit();

    return 0;
}
