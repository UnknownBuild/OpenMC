#version 330 core
out vec4 FragColor;
in vec4 lightColor;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
	vec4 color;
};
uniform Material material;
uniform bool hasColor;
uniform bool hasTexture;

void main() {
    vec4 objectColor = vec4(1.0);
    if (hasTexture) {
        objectColor *= texture(material.diffuse, TexCoords);
    }
    if (hasColor) {
        objectColor *= material.color;
    }
    if (objectColor.a < 0.1) {
        discard;
    }

    FragColor = lightColor * objectColor;
}
