#version 330 core

in vec3 fragPosition; // Reçu du vertex shader
out vec4 fragColor;   // Couleur finale

uniform samplerCube sky;
uniform vec3 lightDir;
uniform vec3 sphereCenter;
uniform float sphereRadius;

uniform sampler2D water;
uniform sampler2D causticTex;
uniform sampler2D tiles;

const vec3 abovewaterColor = vec3(0.25, 1.0, 1.25);
const float IOR_AIR = 1.0;
const float IOR_WATER = 1.333;

void main() {
    vec2 coord = fragPosition.xz * 0.5 + 0.5;
    vec4 waterInfo = texture(water, coord);

    vec3 normal = vec3(waterInfo.b, sqrt(1.0 - dot(waterInfo.ba, waterInfo.ba)), waterInfo.a);
    vec3 eyeDir = normalize(fragPosition);

    vec3 reflectedDir = reflect(eyeDir, normal);
    vec3 refractedDir = refract(eyeDir, normal, IOR_AIR / IOR_WATER);

    vec3 reflectedColor = texture(sky, reflectedDir).rgb * abovewaterColor;
    vec3 refractedColor = texture(sky, refractedDir).rgb * abovewaterColor;

    float fresnel = mix(0.25, 1.0, pow(1.0 - dot(normal, -eyeDir), 3.0));
    fragColor = vec4(mix(refractedColor, reflectedColor, fresnel), 1.0);
}
