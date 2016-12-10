#version 330

in vec2 texCoord;

out vec4 outColor;

uniform sampler2D mySampler;

void main() {
    outColor = texture(mySampler, texCoord);
}
