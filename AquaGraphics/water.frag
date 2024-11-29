#version 330 core

in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D waterTexture;  // Texture de l'eau
uniform vec3 lightColor;         // Couleur de la lumière

void main() {
    // Ajoute une perturbation aux coordonnées de texture
    vec2 distortedTex = texCoord + vec2(0.01 * sin(texCoord.y * 100.0), 0.01 * cos(texCoord.x * 100.0));

    // Couleur finale avec une légère transparence
    vec4 waterColor = texture(waterTexture, distortedTex);
    fragColor = vec4(waterColor.rgb * lightColor, 0.8);
}
