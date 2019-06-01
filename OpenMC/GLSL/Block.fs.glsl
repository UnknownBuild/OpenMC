#version 330 core
#define NR_POINT_LIGHTS 10
out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
		vec4 color;
};
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
uniform Material material;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int pointCount;

uniform bool hasColor;
uniform bool hasTexture;

// 计算点光源
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 objectColor) {
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float objectDis    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * objectDis + light.quadratic * (objectDis * objectDis));
    // 合并结果
    vec4 ambient  = vec4(light.ambient, 1.0);
    vec4 diffuse  = vec4(light.diffuse, 1.0)  * diff;
    vec4 specular = vec4(light.specular, 1.0) * spec;

    return (ambient + diffuse + specular) * objectColor * attenuation;
}

// 计算定向光源
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 objectColor) {
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec4 ambient  = vec4(light.ambient, 1.0);
    vec4 diffuse  = vec4(light.diffuse, 1.0)  * diff;
    vec4 specular = vec4(light.specular, 1.0) * spec;

    return (ambient + diffuse + specular) * objectColor;
}

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
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 计算定向光源
    vec4 result = CalcDirLight(dirLight, norm, viewDir, objectColor);
    // 计算点光源
    for (int i = 0; i < pointCount; i++) {
      result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, objectColor);
    }

    FragColor = result;
}
