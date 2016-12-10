#version 330

in vec4 point;
in vec2 position;
in vec4 variance;
in vec2 rotation;
in float scale;
in vec4 color;
in vec2 texture;

uniform mat4 camera;

out vec2 texCoord;
out vec4 randomColor;

void main() {
    mat4 scaleMatrix = mat4(1.0);
    scaleMatrix[0][0] = 0.01 + scale / 10000;
    scaleMatrix[1][1] = 0.1 + scale / 100;
    
    mat4 positionMatrix = mat4(1.0);
    positionMatrix[0][0] = rotation[0];
    positionMatrix[0][2] = rotation[1];
    positionMatrix[2][2] = rotation[0];
    positionMatrix[2][0] = -rotation[1];
    positionMatrix[3][0] = position.x;
    positionMatrix[3][2] = position.y;

    gl_Position = camera * (positionMatrix * scaleMatrix * point + variance * point.y * point.y);

    randomColor = color * 0.7 * (point.y + 0.8);
    texCoord = texture;
}
