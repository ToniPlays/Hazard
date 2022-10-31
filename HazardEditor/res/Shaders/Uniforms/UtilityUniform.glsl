#ifdef VULKAN_API
layout(std140, set = 0, binding = 0) uniform Utility
#else
layout(std140, binding = 0) uniform Utility
#endif
{
	uniform vec4 CameraPos;
	uniform uint Flags;
} u_Util;