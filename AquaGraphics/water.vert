#version 330 core

in vec3 position; // Position des sommets
in vec2 texCoord; // Coordonnées de texture
out vec2 fragTexCoord; // Coordonnées passées au fragment shader

uniform mat4 modelViewProjection;
uniform float time;

void main() {
    float waveHeight = 0.05 * sin(10.0 * position.x + time) * cos(10.0 * position.z + time);
    vec3 newPosition = position + vec3(0.0, waveHeight, 0.0);

    gl_Position = modelViewProjection * vec4(newPosition, 1.0);
    fragTexCoord = texCoord;
}
