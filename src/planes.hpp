#ifndef PLANES_HPP_
#define PLANES_HPP_

class Planes {
private:
    // it has it's own vao
    void create_geometry_();
public:
    Planes(GLsizei size);

    void resize();
};

#endif
