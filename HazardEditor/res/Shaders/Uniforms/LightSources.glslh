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
	uniform float EnvironmentLod;
	uniform DirectionalLight[8] DirectionalLights;
} u_Lights;