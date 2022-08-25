struct DirectionalLight 
{
	vec4 Direction;
	vec4 Color;
};

layout(std140, binding = 1) uniform Lights 
{
	uniform int DirectionalLightCount;
	uniform int PointLightCount;
	uniform int Padding;
	uniform int Padding2;
	uniform DirectionalLight[16] u_DirectionalLights;
} u_Lights;