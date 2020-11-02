#type Vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 viewProjection;
uniform vec3 offset;


out vec4 pos;
out vec3 TextCoords;

void main() {

	vec4 pos =  vec4(position, 1.0);
	gl_Position = viewProjection * pos;
	TextCoords = position;
}

#type Fragment
#version 330 core

in vec3 TextCoords;

uniform samplerCube skybox;

out vec4 color;

void main() {
	color = texture(skybox, TextCoords);
}