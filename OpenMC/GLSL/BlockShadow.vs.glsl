#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aOffset;

#define NR_POINT_LIGHTS 20
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 lightColor;
out vec2 TexCoords;
out float fogHeight;
out float fogFactor;
out vec4 FragPosLightSpace;

out vec3 FragPos;
out vec3 Normal;


struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int pointCount;

uniform mat4 lightSpaceMatrix;

// 计算点光源
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    // 衰减
    float objectDis   = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * objectDis + light.quadratic * (objectDis * objectDis));
    // 合并结果
    vec4 ambient  = vec4(light.ambient, 1.0);
    vec4 diffuse  = vec4(light.diffuse, 1.0) * diff;
    vec4 specular = vec4(light.specular, 1.0) * spec;

    return (ambient + diffuse + specular) * attenuation;
}

// 计算定向光源
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
    // 合并结果
    vec4 ambient  = vec4(light.ambient, 1.0);
    vec4 diffuse  = vec4(light.diffuse, 1.0) * diff;
    vec4 specular = vec4(light.specular, 1.0) * spec;

    return (ambient + diffuse + specular);
}

const float pi = 3.14159265;
const float e = 2.71828182845904523536028747135266249;

void main() {
	mat4 offsetModel = mat4(1.0, 0.0, 0.0, 0.0,
													0.0, 1.0, 0.0, 0.0,
													0.0, 0.0, 1.0, 0.0,
													aOffset.xyz, 1.0) * model;
    vec4 position = offsetModel * vec4(aPos, 1.0);
	gl_Position = projection * view * position;
	FragPos = vec3(position);
	Normal = mat3(transpose(inverse(offsetModel))) * aNormal;
	TexCoords = aTexCoords;

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	lightColor = CalcDirLight(dirLight, norm, viewDir);
	// 计算点光源 增加 20% 的性能消耗
	for (int i = 0; i < pointCount; i++) {
		lightColor += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	// lightColor = vec4(lightColor.xyz * (aOffset.a / 15.0 + 0.2) ,lightColor.a);
	lightColor = vec4(lightColor.xyz * (1.0) ,lightColor.a);

  float cameraDistance = distance(viewPos, vec3(position));
  // fogFactor = pow(clamp(cameraDistance / fogDistance, 0.0, 1.0), 4.0);
  fogFactor = 1 - pow(e, -pow(cameraDistance * 0.01, 2));

  // float dy = position.y - viewPos.y;
  // float dx = distance(position.xz, viewPos.xz);
  // fogHeight = (atan(dy, dx) + pi / 2) / pi;
  FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}
