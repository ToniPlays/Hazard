#type Vertex
#version 330 core

layout(location = 0) in vec3 position;

out vec3 pos;

void main() {

	pos = position;
	gl_Position = vec4(position, 1.0);
}

#type Fragment
#version 330 core

in vec3 pos;

out vec4 color;

void main() {

	color = vec4(pos * 2, 1.0);
}