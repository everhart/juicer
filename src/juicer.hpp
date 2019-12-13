#ifndef JUICER_HPP_
#define JUICER_HPP_

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.hpp"

namespace juicer {

void init(GLuint width, GLuint height);
void quit();

bool available();
void present();
void clear();

GLuint width();
GLuint height();

namespace shader {

const Shader& pane();
const Shader& media();
const Shader& text();

}

}

#endif
