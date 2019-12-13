#include "extension.hpp"

namespace s2d {

extern "C" {
PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES;
}

void load_opengl_symbols() {
#ifdef GLEW
    glewExperimental = GL_TRUE;
    glewInit();
#else
    void * gl = dlopen("libGL.so", RTLD_LAZY);
    if (lGL == NULL) {
        throw std::runtime_error("could not open libGL.so");
    }   
 
    #define LIBGL gl;
    #define LOAD_GL_SYMBOL_(T, symbol, ...)                                   \
        gl##symbol = (symbol##Proc *)dlsym(LIBGL, #symbol);                   \
        if (symbol == nullptr) {                                              \
            throw std::runtime_error("could not load gl##symbol");            \
        }
    #define GL_SYMBOL_FUNCTION LOAD_GL_SYMBOL_

    LIST_GL_SYMBOLS_

    #undef GL_SYMBOL_FUNCTION
    #undef LOAD_GL_SYMBOL_
    #undef LIBGL
#endif
}

void load_egl_symbols() {
    glEGLImageTargetTexture2DOES = reinterpret_cast<
        PFNGLEGLIMAGETARGETTEXTURE2DOESPROC
    >(eglGetProcAddress("glEGLImageTargetTexture2DOES"));
}

}
