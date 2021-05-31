#version 450 core

in VertexOut {
	vec3 worldPos;
	vec4 color;
	vec3 normal;
	vec2 texCoord;
} vsIn;


uniform samplerCube envMap;
uniform sampler2D albedoMap;
uniform vec4 u_color;

layout(std140, binding = 0) uniform Camera 
{
	mat4 u_ViewProjection;
	vec3 u_CameraPos;
	float u_Gamma;
};


layout(location = 0) out vec4 color;

vec4 mapHDR(vec3 color) {
	vec3 mapped = color / (color + vec3(1.0));
	mapped = pow(mapped, vec3(1.0 / u_Gamma));
	return vec4(mapped, 1.0);
}

vec3 CalculateFresnel(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() 
{
	vec3 norm = normalize(vsIn.normal);
	vec3 viewDir = normalize(vsIn.worldPos - u_CameraPos);
	vec3 modelDir = normalize(u_CameraPos - vsIn.worldPos);

	vec3 reflectVector = reflect(viewDir, norm);

	vec4 albedo = texture(albedoMap, vsIn.texCoord);

	float cosTheta = max(dot(modelDir, norm), 0.0);

	color = albedo * u_color * vsIn.color;
}