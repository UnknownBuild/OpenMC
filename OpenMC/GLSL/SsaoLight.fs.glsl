#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gAlbedo;
uniform sampler2D ssao;


void main() {
    vec4 Diffuse = texture(gAlbedo, TexCoords);
    float AmbientOcclusion = texture(ssao, TexCoords).r;
    FragColor = AmbientOcclusion * Diffuse;
}
