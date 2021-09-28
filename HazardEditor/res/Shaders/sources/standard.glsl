#type Vertex
#version 450

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec4 v_Color;

layout(location = 0) out vec4 f_color;

void main() {

	f_color = v_Color;
	gl_Position = vec4(v_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 f_color;

layout(location = 0) out vec4 color;

void main() {
	color = f_color;
}
