#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aOffset;

layout (location = 4) in vec4 aoTop;
layout (location = 5) in vec4 aoBottom;
layout (location = 6) in vec4 aoLeft;
layout (location = 7) in vec4 aoRight;
layout (location = 8) in vec4 aoFront;
layout (location = 9) in vec4 aoBack;

#define NR_POINT_LIGHTS 20
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 lightColor;
out vec2 TexCoords;
out float fogHeight;
out float fogFactor;

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

const float pi = 3.14159265;
const float e = 2.71828182845904523536028747135266249;

void main() {
	mat4 offsetModel = mat4(1.0, 0.0, 0.0, 0.0,
													0.0, 1.0, 0.0, 0.0,
													0.0, 0.0, 1.0, 0.0,
													aOffset.xyz, 1.0) * model;
    vec4 position = offsetModel * vec4(aPos, 1.0);
	gl_Position = projection * view * position;
	vec3 FragPos = vec3(position);
	vec3 Normal = mat3(transpose(inverse(offsetModel))) * aNormal;
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

    if (gl_VertexID >= 0 && gl_VertexID < 4) {
        lightColor = vec4(lightColor.xyz * aoFront[gl_VertexID % 4] ,lightColor.a);
    }
    if (gl_VertexID >= 4 && gl_VertexID < 8) {
        lightColor = vec4(lightColor.xyz * aoBack[gl_VertexID % 4] ,lightColor.a);
    }
    if (gl_VertexID >= 8 && gl_VertexID < 12) {
        lightColor = vec4(lightColor.xyz * aoLeft[gl_VertexID % 4] ,lightColor.a);
    }
    if (gl_VertexID >= 12 && gl_VertexID < 16) {
        lightColor = vec4(lightColor.xyz * aoRight[gl_VertexID % 4] ,lightColor.a);
    }
    if (gl_VertexID >= 16 && gl_VertexID < 20) {
        lightColor = vec4(lightColor.xyz * aoBottom[gl_VertexID % 4] ,lightColor.a);
    }
    if (gl_VertexID >= 20 && gl_VertexID < 24) {
        lightColor = vec4(lightColor.xyz * aoTop[gl_VertexID % 4] ,lightColor.a);
    }

}
