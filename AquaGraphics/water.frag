#version 330 core

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D waterTexture;
uniform vec3 lightColor;

void main() {
    vec2 distortedTex = fragTexCoord + vec2(0.01 * sin(fragTexCoord.y * 100.0), 0.01 * cos(fragTexCoord.x * 100.0));
    vec4 waterColor = texture(waterTexture, distortedTex);

    fragColor = vec4(waterColor.rgb * lightColor, 0.8);
}
