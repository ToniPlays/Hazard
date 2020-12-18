#type Vertex
#version 330 core

layout(location = 0) in vec3 position;

void main() {
	gl_Position = vec4(position, 1.0);
}

#type Fragment
#version 330 core
out vec4 color;

void main() {
	color = vec4(0.3f, 0.6f, 0.3f, 1.0f);
}