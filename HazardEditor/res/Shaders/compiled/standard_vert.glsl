#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec2 v_tex_coords;
layout(location = 3) in float v_tex_index;

uniform mat4 viewProjection;

out vec4 f_color;
out vec2 texCoords;
flat out float textureIndex;

void main() {

	f_color = v_color;
	texCoords = v_tex_coords;
	textureIndex = v_tex_index;

	gl_Position = viewProjection * vec4(v_position, 1.0);
}
