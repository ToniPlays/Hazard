#version 450 core

in vec4 f_color;
in vec3 f_normal;
in vec2 texCoords;
in vec3 reflectedVector;
in float gamma;

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
	mapped = pow(mapped, vec3(1.0 / gamma));
	return vec4(mapped, 1.0);
}

void main() 
{
	vec4 reflectedColor = texture(envMap, reflectedVector);
	vec4 albedo = texture(albedoMap, texCoords);

	color = albedo * u_color;
}