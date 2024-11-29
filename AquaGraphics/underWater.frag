#version 330 core

in vec3 fragPosition;
out vec4 fragColor;

uniform samplerCube sky;
uniform vec3 lightDir;
uniform vec3 sphereCenter;
uniform float sphereRadius;

uniform sampler2D water;
uniform sampler2D causticTex;

const vec3 underwaterColor = vec3(0.4, 0.9, 1.0);
const float IOR_AIR = 1.0;
const float IOR_WATER = 1.333;

void main() {
    vec2 coord = fragPosition.xz * 0.5 + 0.5;
    vec4 waterInfo = texture(water, coord);

    vec3 normal = vec3(-waterInfo.b, sqrt(1.0 - dot(waterInfo.ba, waterInfo.ba)), -waterInfo.a);
    vec3 eyeDir = normalize(fragPosition);

    vec3 reflectedDir = reflect(eyeDir, normal);
    vec3 refractedDir = refract(eyeDir, normal, IOR_WATER / IOR_AIR);

    vec3 reflectedColor = texture(sky, reflectedDir).rgb * underwaterColor;
    vec3 refractedColor = reflectedColor * vec3(0.8, 1.0, 1.1);

    float fresnel = mix(0.5, 1.0, pow(1.0 - dot(normal, -eyeDir), 3.0));
    fragColor = vec4(mix(reflectedColor, refractedColor, fresnel), 1.0);
}

