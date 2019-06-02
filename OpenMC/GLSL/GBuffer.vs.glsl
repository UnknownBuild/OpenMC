#version 330 core
#define NR_POINT_LIGHTS 20

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aOffset;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 lightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

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

void main() {
	mat4 offsetModel = mat4(1.0, 0.0, 0.0, 0.0,
													0.0, 1.0, 0.0, 0.0,
													0.0, 0.0, 1.0, 0.0,
													aOffset.xyz, 1.0) * model;
	// gl_Position = projection * view * offsetModel * vec4(aPos, 1.0);
	// FragPos = vec3(offsetModel * vec4(aPos, 1.0));
	// Normal = mat3(transpose(inverse(offsetModel))) * aNormal;
	// TexCoords = aTexCoords;

	vec4 mViewPos = view * offsetModel * vec4(aPos, 1.0f);
	FragPos = mViewPos.xyz;
	gl_Position = projection * mViewPos;
	TexCoords = aTexCoords;

	mat3 normalMatrix = transpose(inverse(mat3(view * offsetModel)));
	Normal = normalMatrix * aNormal;

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	lightColor = CalcDirLight(dirLight, norm, viewDir);
	// 计算点光源 增加 20% 的性能消耗
	for (int i = 0; i < pointCount; i++){
		lightColor += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
}
