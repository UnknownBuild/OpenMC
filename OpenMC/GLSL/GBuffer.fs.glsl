#version 330 core
layout (location = 0) out vec4 gPositionDepth;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 lightColor;

const float NEAR = 0.1; // Projection matrix's near plane distance
const float FAR = 256.0; // Projection matrix's far plane distance

struct Material {
    sampler2D diffuse;
	vec4 color;
};
uniform Material material;
uniform bool hasColor;
uniform bool hasTexture;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
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
    // Store the fragment position vector in the first gbuffer texture
    gPositionDepth.xyz = FragPos;
    // And store linear depth into gPositionDepth's alpha component
    gPositionDepth.a = LinearizeDepth(gl_FragCoord.z); // Divide by FAR if you need to store depth in range 0.0 - 1.0 (if not using floating point colorbuffer)
    // Also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // And the diffuse per-fragment color
    gAlbedoSpec = objectColor * lightColor; // Currently all objects have constant albedo color
}
