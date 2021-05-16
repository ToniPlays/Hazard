#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec2 v_texCoords;

uniform mat4 viewProjection;
uniform mat4 model;
uniform vec3 cameraPos;

out vec4 f_color;
out vec3 f_normal;
out vec2 texCoords;
out vec3 reflectedVector;

void main() 
{
	vec4 worldPos = model * vec4(v_position, 1.0);
	gl_Position = viewProjection * worldPos;

	f_color = v_color;
	f_normal = mat3(model) * v_normal;
	texCoords = v_texCoords;

	vec3 unitNormal = normalize(f_normal);
	vec3 viewVector = normalize(worldPos.xyz - cameraPos);

	reflectedVector = reflect(viewVector, unitNormal);
}
