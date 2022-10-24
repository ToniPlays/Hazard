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


	vec4 inversedPos = position;
	v_Position = (u_Camera.InverseViewProjection * inversedPos).xyz;
}

#type Fragment
#version 450

#include "Uniforms/CameraUniform.glsl"
#include "Uniforms/LightSources.glsl"

layout(location = 0) in vec3 v_Position;

layout(binding = 0) uniform samplerCube u_CubeMap;

layout(location = 0) out vec4 OutputColor;

void main() 
{
	float ao = u_Lights.SkyLightIntensity;

	vec3 color = texture(u_CubeMap, v_Position).rgb;
	OutputColor = vec4(color, 1.0);
}