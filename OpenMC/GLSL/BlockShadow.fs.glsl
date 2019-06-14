#version 330 core
out vec4 FragColor;
in vec4 lightColor;
in vec2 TexCoords;
in float fogHeight;
in float fogFactor;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    sampler2D diffuse;
	vec4 color;
};
uniform Material material;
uniform bool hasColor;
uniform bool hasTexture;

in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;
uniform vec3 lightPos;

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
	vec3 lightDir = normalize(lightPos - FragPos);

  vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  float closestDepth = texture(shadowMap, projCoords.xy).r;
  float currentDepth = projCoords.z;
	// float bias = 0.001;
	float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.003);
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -2; x <= 2; ++x)
	{
	  for(int y = -2; y <= 2; ++y)
	  {
		float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
		shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
	  }
	}
	shadow /= 25.0;

    FragColor = mix((1.0 - shadow) * lightColor * objectColor, vec4(0.6, 0.8, 0.8, 1.0), fogFactor);
}
