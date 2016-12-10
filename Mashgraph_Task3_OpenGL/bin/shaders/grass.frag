#version 330

in vec2 texCoord;
in vec4 randomColor;

out vec4 outColor;

uniform sampler2D mySampler;

void main() {
    outColor = texture(mySampler, texCoord) * randomColor;
}
