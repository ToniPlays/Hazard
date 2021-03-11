#type Vertex
#version 330 core
layout(location = 0) in vec3 v_position;

uniform mat4 viewProjection;
out vec3 texCoords;

void main() {
	
	texCoords = v_position;
	vec4 pos = viewProjection * vec4(v_position, 1.0);
	gl_Position = pos.xyww;
}


#type Fragment
#version 330 core

in vec3 texCoords;

uniform samplerCube SkyboxCubemap;

out vec4 color;

void main() {
	color = texture(SkyboxCubemap, texCoords);
}