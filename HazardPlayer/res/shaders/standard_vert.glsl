
#version 450 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec4 v_color;
layout (location = 2) in vec2 v_tex_coords;
layout (location = 3) in float v_tex_index;

layout(std140, binding = 0) uniform Camera 
{
	mat4 u_ViewProjection;
	vec3 u_CameraPos;
	float u_Gamma;
};

layout (location = 0) out vec4 f_color;
layout (location = 1) out vec2 texCoords;
layout (location = 2) out flat float textureIndex;

void main() 
{
	f_color = v_color;
	texCoords = v_tex_coords;
	textureIndex = v_tex_index;

	gl_Position = u_ViewProjection * vec4(v_position, 1.0);
}