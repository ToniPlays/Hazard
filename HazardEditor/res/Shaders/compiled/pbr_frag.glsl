#version 450 core

in VertexOut {
	vec3 worldPos;
	vec3 viewDir;
	vec4 color;
	vec3 normal;
	vec3 screenNormal;
	vec2 texCoord;
} vsIn;


uniform samplerCube envMap;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform vec4 u_color;
uniform float u_metallic;
uniform float u_smoothness;

uniform vec3 lightPositions[1];
uniform vec3 lightColors[1];

layout(std140, binding = 0) uniform Camera 
{
	mat4 u_ViewProjection;
	vec3 u_CameraPos;
	float u_Gamma;
};


layout(location = 0) out vec4 color;

vec3 mapHDR(vec3 color) {
	vec3 mapped = color / (color + vec3(1.0));
	mapped = pow(mapped, vec3(1.0 / u_Gamma));
	return mapped;
}

vec3 CalculateFresnel(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

void main() 
{
	vec3 norm = normalize(vsIn.normal);
	vec3 viewDir = normalize(vsIn.viewDir);

	vec3 reflectVector = reflect(-vsIn.viewDir, vsIn.screenNormal);
	vec3 environmentColor = texture(envMap, reflectVector).rgb;

	vec4 albedo = texture(albedoMap, vsIn.texCoord);

	float cosTheta = max(dot(norm, viewDir), 0.0);
	vec3 fresnel = CalculateFresnel(cosTheta, vec3(1.0)) * mapHDR(environmentColor.rgb);


	color = mix(albedo, albedo * vec4(fresnel, 1.0), u_metallic) * u_color;
}