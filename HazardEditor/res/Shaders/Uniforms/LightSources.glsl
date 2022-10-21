struct DirectionalLight 
{
	vec4 Direction;
	vec4 Color;
};

#ifdef VULKAN_API
layout(std140, set = 0, binding = 1) uniform Lights 
#else
layout(std140, binding = 1) uniform Lights 
#endif
{
	uniform int DirectionalLightCount;
	uniform int PointLightCount;
	uniform float SkyLightIntensity;
	uniform int Padding2;
	uniform DirectionalLight[8] u_DirectionalLights;
} u_Lights;