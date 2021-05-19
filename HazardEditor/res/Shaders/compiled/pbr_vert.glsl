#version 450 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec2 v_texCoords;

layout(std140, binding = 0) uniform Camera 
{
	mat4 u_ViewProjection;
	vec3 u_CameraPos;
	float u_Gamma;
};

uniform mat4 u_Model;

out vec4 f_color;
out vec3 f_normal;
out vec2 texCoords;
out vec3 reflectedVector;
out float gamma;

void main() 
{
	vec4 worldPos = u_Model * vec4(v_position, 1.0);
	gl_Position = u_ViewProjection * worldPos;

	f_color = v_color;
	f_normal = mat3(u_Model) * v_normal;
	texCoords = v_texCoords;

	vec3 unitNormal = normalize(f_normal);
	vec3 viewVector = normalize(worldPos.xyz - u_CameraPos);

	reflectedVector = reflect(viewVector, unitNormal);
	gamma = u_Gamma;
}