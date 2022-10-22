#ifdef VULKAN_API
layout(std140, set = 1, binding = 1) uniform Grid 
#else
layout(std140, binding = 4) uniform Grid 
#endif
{
	uniform float Scale;
	uniform float ScaleFading;

} u_Grid;