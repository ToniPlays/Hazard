#type Vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

out vec2 coords;

void main() {
	gl_Position = projection * view * transform * vec4(position, 1.0);
	coords = textureCoords;
}

#type Fragment
#version 330 core

in vec2 coords;

uniform sampler2D T_texture;
uniform float test;
uniform int gradient;

out vec4 color;

void main() {
	
	vec4 c = texture(T_texture, coords);

	c = vec4(c.g, c.g, c.g, 1.0);

	if(gradient == 1)
		c = c  * vec4(coords, test, 1.0);
	color = c;
}