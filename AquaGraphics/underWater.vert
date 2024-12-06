#version 330 core

in vec3 position; // Position des sommets
out vec3 fragPosition; // Position envoyée au fragment shader

uniform sampler2D water; // Texture de l'eau
uniform mat4 modelViewProjection;

void main() {
    vec2 coord = position.xz * 0.5 + 0.5;
    vec4 waterInfo = texture(water, coord);

    fragPosition = vec3(position.x, waterInfo.r, position.z);
    gl_Position = modelViewProjection * vec4(fragPosition, 1.0);
}
