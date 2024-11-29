#version 330 core

layout(location = 0) in vec3 position;
out vec3 fragPosition;

uniform vec3 sphereCenter;
uniform float sphereRadius;
uniform mat4 modelViewProjection;

void main() {
    fragPosition = sphereCenter + position * sphereRadius;
    gl_Position = modelViewProjection * vec4(fragPosition, 1.0);
}

