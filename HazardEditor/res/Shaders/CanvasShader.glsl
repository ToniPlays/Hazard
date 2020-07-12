#type Vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

out vec4 v_color;

void main() {
	
	gl_Position = projection * view * transform * vec4(position, 1.0);
	v_color = color;
}

#type Fragment
#version 330 core

in vec4 v_color;

out vec4 out_color;

void main() {
	out_color = v_color;
}