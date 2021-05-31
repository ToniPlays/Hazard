#version 450 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec2 v_texCoords;

layout(std140, binding = 0) uniform Camera 
{
	mat4 u_ViewProjection;
	vec3 u_CameraPos;
	float u_Gamma;
};

uniform mat4 u_Model;

out VertexOut {
	vec3 worldPos;
	vec3 viewDir;
	vec4 color;
	vec3 normal;
	vec2 texCoord;
} vsOut;

void main() 
{
	vec3 worldPos = vec3(u_Model * vec4(v_position, 1.0));
	gl_Position = u_ViewProjection * vec4(worldPos, 1.0);

	vsOut.worldPos = worldPos;
	vsOut.viewDir = u_CameraPos - worldPos;
	vsOut.color = v_color;
	vsOut.normal = v_normal;
	vsOut.texCoord = v_texCoords;
}
