#version 450 core

layout(location = 0) in vec3 v_position;

layout(std140, binding = 0) uniform Camera 
{
	mat4 u_ViewProjection;
	vec3 u_CameraPos;
	float u_Gamma;
};

out vec3 texCoords;
out float gamma;

void main() {
	
	texCoords = v_position;
	vec4 pos = u_ViewProjection * vec4(v_position, 1.0);
	gl_Position = pos.xyww;
	gamma = u_Gamma;
}
