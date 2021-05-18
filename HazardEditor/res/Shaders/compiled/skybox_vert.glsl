#version 450 core

layout(location = 0) in vec3 v_position;

uniform mat4 viewProjection;
out vec3 texCoords;

void main() {
	
	texCoords = v_position;
	vec4 pos = viewProjection * vec4(v_position, 1.0);
	gl_Position = pos.xyww;
}
