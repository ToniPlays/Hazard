#type Vertex
#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec3 v_tex_coords;

uniform mat4 viewProjection;

out vec4 f_color;
out vec2 texCoords;
flat out int textureIndex;

void main() {

	f_color = v_color;

	texCoords = v_tex_coords.xy;
	textureIndex = int(v_tex_coords.z);

	gl_Position = viewProjection * vec4(v_position, 1.0);
}

#type Fragment
#version 330 core

in vec4 f_color;
in vec2 texCoords;
flat in int textureIndex;

uniform sampler2D u_textures[8];

out vec4 color;

void main() {
	color = f_color * texture(u_textures[textureIndex], texCoords);
}