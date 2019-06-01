#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material {
    sampler2D diffuse;
	vec4 color;
};

uniform Material material;

uniform bool hasColor;
uniform bool hasTexture;

void main() {
    gPosition = vec4(FragPos, 1.0);
    gNormal = vec4(normalize(Normal), 1.0) ;

    vec4 color = vec4(1.0);
    if (hasTexture) {
        color *= texture(material.diffuse, TexCoords);
    }
    if (hasColor) {
        color *= material.color;
    }
    if (color.a < 0.1) {
        discard;
    }
    gColor = color;
}
