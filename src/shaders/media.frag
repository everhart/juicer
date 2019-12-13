#version 330 core

uniform sampler2D sampler;
in vec2 coord;

void main() {
    // gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    gl_FragColor = texture(sampler, coord);
}
