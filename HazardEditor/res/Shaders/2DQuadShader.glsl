#type Vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 viewProjection;

out vec4 v_color;

void main() {
	vec4 worldPos = viewProjection * vec4(position, 1.0);

	v_color = color;

	gl_Position = worldPos;
}

#type Fragment
#version 330 core

in vec4 v_color;
out vec4 color;

void main() {

	color = v_color;
}