#version 330 core

in vec3 fragPosition;
out vec4 fragColor;

uniform vec3 lightDir;
uniform sampler2D water;
uniform vec3 sphereCenter;
uniform float sphereRadius;

const vec3 underwaterColor = vec3(0.4, 0.9, 1.0);

vec3 getSphereColor(vec3 point) {
    vec3 normal = normalize(point - sphereCenter);
    float diffuse = max(0.0, dot(-lightDir, normal));
    vec3 color = vec3(0.5) + diffuse * vec3(0.2, 0.4, 0.6);

    vec4 waterInfo = texture(water, point.xz * 0.5 + 0.5);
    if (point.y < waterInfo.r) {
        color *= underwaterColor * 1.2;
    }

    return color;
}

void main() {
    fragColor = vec4(getSphereColor(fragPosition), 1.0);
}
