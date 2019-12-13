#version 330 core
layout (location = 0) in vec3 xyz; 
layout (location = 1) in vec2 uv; 

uniform mat4 modelview;
uniform mat4 projection;
out vec2 coord;

void main() {
    gl_Position = projection * model * vec4(xyz, 1.0f);
    coord = uv;
}
