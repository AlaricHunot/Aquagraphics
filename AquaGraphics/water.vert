#version 330 core

layout(location = 0) in vec3 aPosition;  // Position des sommets
layout(location = 1) in vec2 aTexCoord;  // Coordonnées de texture

out vec2 texCoord;  // Coordonnées passées au fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;  // Temps pour animer les vagues

void main() {
    // Calcul d'une onde simple avec sin/cos
    float waveHeight = 0.05 * sin(10.0 * aPosition.x + time) * cos(10.0 * aPosition.z + time);

    // Déplace les sommets selon l'onde
    vec3 newPosition = aPosition + vec3(0.0, waveHeight, 0.0);

    // Position finale du sommet
    gl_Position = projection * view * model * vec4(newPosition, 1.0);
    texCoord = aTexCoord;
}
