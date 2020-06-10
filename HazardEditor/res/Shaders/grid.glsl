#type Vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

void main() {

	vec4 wp = transform * vec4(position, 1.0);
	vec4 p = projection * view * wp;
	gl_Position = p;
}

#type Fragment
#version 330 core
out vec4 color;

void main() {
	
	color = vec4(0.3, 0.3, 0.3, 1.0);
}