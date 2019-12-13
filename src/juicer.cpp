#include "juicer.hpp"

#include <stdexcept>
#include <GLFW/glfw3.h>

namespace juicer {

static GLuint width_ = 0;
static GLuint height_ = 0;
static GLFWwindow * window_ = NULL;

static Shader pane_;
static Shader media_;
static Shader text_;

void init(GLuint w, GLuint h) {
    width_ = w;
    height_ = h;

    if (glfwInit() == 0) {
        throw std::runtime_error("could not initialize glfw");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window_ = glfwCreateWindow(w, h, "juicer", NULL, NULL);
    if (window_ == NULL) {
        throw std::runtime_error("could not initialize glfw window");
    }
    glfwMakeContextCurrent(window_);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("could not initialize glew");
    }

    // configure the current opengl context for 2d rendering
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport(0, 0, width_, height_);

    // initialize global shader variables
    pane_ = Shader("src/shaders/graphic.vert", "src/shaders/graphic.frag");
    media_ = Shader("src/shaders/media.vert", "src/shaders/media.frag");
    text_ = Shader("src/shaders/text.vert", "src/shaders/text.frag");
}

void quit() {
    glfwDestroyWindow(window_);
    glfwTerminate();
}

bool available() {
    return !glfwWindowShouldClose(window_);
}

void present() {
    glfwSwapBuffers(window_);
}

void clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

GLuint width() {
    return width_;
}

GLuint height() {
    return height_;
}

namespace shader {

const Shader& pane() {
    return pane_;
}

const Shader& media() {
    return media_;
}

const Shader& text() {
    return text_;
}

}
}
