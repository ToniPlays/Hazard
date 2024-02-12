#type Vertex
#version 450

#include "Uniforms/CameraUniform.glslh"

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

layout(location = 0) in vec3 v_Position;

layout(set = 0, binding = 1) uniform samplerCube u_CubeMap;

layout(push_constant) uniform PushConstant
{
	float LodLevel;
	float Intensity;
} u_PushConstants;

layout(location = 0) out vec4 OutputColor;
layout(location = 1) out uint EntityID;

#include "Utils/PostProcessing.glslh"

const float gamma = 2.2;

void main()
{
	int levelTexels = textureQueryLevels(u_CubeMap);
	vec3 color = textureLod(u_CubeMap, v_Position, (u_PushConstants.LodLevel * levelTexels)).rgb;
	color *= u_PushConstants.Intensity;
    color = GammaCorrect(color, gamma);

	OutputColor = vec4(ACESTonemap(color), 1.0);

	EntityID = 0;
}
