#type Vertex
#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;

uniform mat4 viewProjection;

out vec4 f_color;

void main() {

	f_color = v_color;
	gl_Position = viewProjection * vec4(v_position, 1.0);
}

#type Fragment
#version 330 core

in vec4 f_color;

out vec4 color;

void main() {

	color = f_color;
}