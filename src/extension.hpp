#ifndef EXTENSION_HPP_
#define EXTENSION_HPP_

/**
 * Author's Note
 *
 * I abhor macros for all the obvious reasons. However, the meta programming
 * involved in OpenGL extension loading has lead me to consider them a
 * justifiable evil in this specific case. Nonetheless, please proceed
 * cautiously when editing this file.
**/

#ifdef GLEW
#include <GL/glew.h>
#else
#include <dlfcn.h>

//! list of all GL symbols to load
#define LIST_GL_SYMBOLS_                                                      \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glAttachShader,                                                 \
        GLuint program, GLuint shader                                         \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glBindBuffer,                                                   \
        GLenum target, GLuint buffer                                          \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glBindFramebuffer, GLenum target, GLuint framebuffer            \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glBufferData,                                                   \
        GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage      \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glBufferSubData,                                                \
        GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data  \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        GLenum, glCheckFramebufferStatus,                                     \
        GLenum target                                                         \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glClearBufferfv,                                                \
        GLenum buffer, GLint drawbuffer, const GLfloat * value                \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glCompileShader,                                                \
        GLuint shader                                                         \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        GLuint, glCreateProgram,                                              \
        void                                                                  \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        GLuint, glCreateShader,                                               \
        GLenum type                                                           \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glDeleteBuffers,                                                \
        GLsizei n, const GLuint *buffers                                      \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glDeleteFramebuffers,                                           \
        GLsizei n, const GLuint *framebuffers                                 \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glEnableVertexAttribArray,                                      \
        GLuint index                                                          \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glDrawBuffers,                                                  \
        GLsizei n, const GLenum *bufs                                         \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void,   glFramebufferTexture2D,                                       \
        GLenum  target,                                                       \
        GLenum  attachment,                                                   \
        GLenum  textarget,                                                    \
        GLuint  texture,                                                      \
        GLint   level                                                         \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glGenBuffers,                                                   \
        GLsizei n, GLuint *buffers                                            \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glGenFramebuffers,                                              \
        GLsizei n, GLuint * framebuffers                                      \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        GLint, glGetAttribLocation,                                           \
        GLuint program, const GLchar *name                                    \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glGetShaderInfoLog,                                             \
        GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog      \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glGetShaderiv,                                                  \
        GLuint shader, GLenum pname, GLint *params                            \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        GLint, glGetUniformLocation,                                          \
        GLuint program, const GLchar *name                                    \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glLinkProgram,                                                  \
        GLuint program                                                        \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void,           glShaderSource,                                       \
        GLuint          shader,                                               \
        GLsizei         count,                                                \
        const GLchar    * const * string,                                     \
        const GLint     * length                                              \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glUniform1i,                                                    \
        GLint location, GLint v0                                              \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(void, glUniform1f, GLint location, GLfloat v0)        \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glUniform2f, GLint location, GLfloat v0, GLfloat v1             \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glUniform4f,                                                    \
        GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3        \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void,           glUniformMatrix4fv,                                   \
        GLint           location,                                             \
        GLsizei         count,                                                \
        GLboolean       transpose,                                            \
        const GLfloat   * value                                               \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void, glUseProgram,                                                   \
        GLuint program                                                        \
    )                                                                         \
    GL_SYMBOL_FUNCTION_(                                                      \
        void,           glVertexAttribPointer,                                \
        GLuint          index,                                                \
        GLint           size,                                                 \
        GLenum          type,                                                 \
        GLboolean       normalized,                                           \
        GLsizei         stride,                                               \
        const GLvoid    * pointer                                             \
    )

//! macro for listing all GL Function Pointer types
#define DECLARE_GL_SYMBOL_(T, symbol, ...)                                    \
    typedef T gl##symbol##Proc(__VA_ARGS__);                                  \
    extern symbol##Proc * gl##symbol;

#define GL_SYMBOL_FUNCTION DECLARE_GL_SYMBOL_
    LIST_GL_SYMBOLS_
#undef GL_SYMBOL_FUNCTION

#endif

#include <EGL/egl.h>

extern "C" {
typedef void * GLeglImageOES;
typedef void (* PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)(GLenum, GLeglImageOES);
extern PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES;
}

namespace s2d {

void load_egl_symbols();
void load_opengl_symbols();

}

#endif
