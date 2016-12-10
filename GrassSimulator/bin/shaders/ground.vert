#version 330


in vec2 texture;
in vec4 point;

uniform mat4 camera;

out vec2 texCoord;

void main() {
    gl_Position = camera * point;
    texCoord = texture;
}
