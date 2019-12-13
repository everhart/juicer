#version 330 core

uniform sampler2D sampler;
uniform vec4 color;
in vec2 coord;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(sampler, coord).r);
    gl_FragColor = color * sampled;
}
