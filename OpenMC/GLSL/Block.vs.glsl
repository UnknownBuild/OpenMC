#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aOffset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main() {
	mat4 offsetModel = mat4(1.0, 0.0, 0.0, 0.0,
													0.0, 1.0, 0.0, 0.0,
													0.0, 0.0, 1.0, 0.0,
													aOffset.x, aOffset.y, aOffset.z, 1.0);
	offsetModel = offsetModel * model;
	gl_Position = projection * view * offsetModel * vec4(aPos, 1.0);
	FragPos = vec3(offsetModel * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(offsetModel))) * aNormal;
	TexCoords = aTexCoords;
}
