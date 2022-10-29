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
	v_Position = (mat4(mat3(inverse(u_Camera.View))) * inverse(u_Camera.Projection) * inversedPos).xyz;
}

#type Fragment
#version 450

#include "Uniforms/CameraUniform.glsl"
#include "Uniforms/LightSources.glsl"

layout(location = 0) in vec3 v_Position;

layout(set = 1, binding = 1) uniform samplerCube u_CubeMap;

layout(location = 0) out vec4 OutputColor;

void main() 
{
	int levelTexels = textureQueryLevels(u_CubeMap);
	vec3 color = textureLod(u_CubeMap, v_Position, (u_Lights.EnvironmentLod * levelTexels)).rgb;
	OutputColor = vec4(color * u_Lights.SkyLightIntensity, 1.0);
}