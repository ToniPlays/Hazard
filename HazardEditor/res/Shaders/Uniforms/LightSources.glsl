struct DirectionalLight 
{
	vec4 Direction;
	vec4 Color;
};

#ifdef VULKAN_API
layout(std140, set = 0, binding = 2) uniform Lights 
#else
layout(std140, binding = 2) uniform Lights 
#endif
{
	uniform int DirectionalLightCount;
	uniform int PointLightCount;
	uniform int Padding;
	uniform int Padding2;
	uniform DirectionalLight[16] u_DirectionalLights;
} u_Lights;