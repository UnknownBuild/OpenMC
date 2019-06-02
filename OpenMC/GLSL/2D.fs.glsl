#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;

void main() {
    if (texture(image, TexCoords).a < 0.01) {
        discard;
    }
    color = texture(image, TexCoords);
}
