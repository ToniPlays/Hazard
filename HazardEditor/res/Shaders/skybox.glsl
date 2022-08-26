#type Vertex
#version 450

#include "Uniforms/CameraUniform.glsl"

layout(location = 0) out vec3 v_Position;

vec3 quadPos[6] = vec3[](
	vec3( 1,  1, 0), vec3(-1, -1, 0), vec3(-1,  1, 0),
	vec3(-1, -1, 0), vec3( 1,  1, 0), vec3( 1, -1, 0)
);

void main() 
{
	vec4 position = vec4(quadPos[gl_VertexIndex].xy, 1.0, 1.0);
	gl_Position = position;

	v_Position = (u_Camera.u_InverseViewProjection * position).xyz;
}

#type Fragment
#version 450

#include "Uniforms/CameraUniform.glsl"

layout(location = 0) in vec3 v_Position;

layout(binding = 0) uniform samplerCube u_RadianceMap;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

void main() 
{
	gPosition = vec4(0.0);
	gNormal = vec4(0.0);
	gAlbedoSpec = texture(u_RadianceMap, v_Position);
	gAlbedoSpec = vec4(0.8, 0.8, 0.4, 1.0);
}