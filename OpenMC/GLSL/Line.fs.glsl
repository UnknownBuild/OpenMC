#version 330 core
out vec4 FragColor;
uniform vec3 drawColor;

void main() {
	FragColor = vec4(drawColor, 1.0);
}
