#type Vertex
#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec3 v_normals;
layout(location = 3) in vec2 v_texCoords;

uniform mat4 viewProjection;

out vec4 f_color;
out vec3 f_normal;
out vec2 texCoords;

void main() {

	f_color = v_color;
	f_normal = v_normals;
	texCoords = v_texCoords;
	gl_Position = viewProjection * vec4(v_position, 1.0);
}

#type Fragment
#version 330 core

in vec4 f_color;
in vec3 f_normal;
in vec2 texCoords;

out vec4 color;

void main() {



	color = f_color * vec4(f_normal, 1.0);
}