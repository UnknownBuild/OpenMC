#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
		vec3 color;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
		vec3 color;
};

#define NR_POINT_LIGHTS 4

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 strength;

uniform bool hasColor;
uniform bool hasTexture;

void main() {
  vec3 ambient = strength.x * dirLight.color;
	if (hasTexture) {
		ambient *= texture(material.diffuse, TexCoords).rgb;
	} else {
		ambient *= dirLight.ambient;
	}

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-dirLight.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = strength.y * diff * dirLight.color;
	if (hasTexture) {
		diffuse *= texture(material.diffuse, TexCoords);
	} else  {
		// diffuse *= dirLight.diffuse;
	}

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
	vec3 specular = strength.z * spec * dirLight.color;
	if (hasTexture) {
		// specular *= texture(material.specular, TexCoords).rgb;
	} else {
		specular *= dirLight.specular;
	}

	vec3 result = ambient + diffuse + specular;

	if (hasTexture) {
		// result *= texture(material.diffuse, TexCoords);
	} else {
		result *= dirLight.diffuse;
	}

	FragColor = vec4(result, 1.0);
}
