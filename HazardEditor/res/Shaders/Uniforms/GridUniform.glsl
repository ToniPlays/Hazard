#ifdef VULKAN_API
layout(std140, set = 1, binding = 1) uniform Grid 
#else
layout(std140, binding = 4) uniform Grid 
#endif
{
	uniform float u_ZNear;
	uniform float u_ZFar;
	uniform float u_Scale;
	uniform float u_ScaleFading;

} u_Grid;