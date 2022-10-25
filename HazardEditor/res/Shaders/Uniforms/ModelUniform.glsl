#ifdef VULKAN_API
layout(std140, set = 1, binding = 0) uniform Model 
#else
layout(std140, binding = 3) uniform Model 
#endif
{
	uniform mat4 Transform;
	uniform float Metalness;
	uniform float Roughness;
	uniform int Flags;
} u_Model;