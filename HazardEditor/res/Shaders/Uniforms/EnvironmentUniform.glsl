#ifdef VULKAN_API
layout(std140, set = 0, binding = 0) uniform Environment
#else
layout(std140, binding = 0) uniform Environment
#endif
	uniform samplerCube u_Irradiance;
} u_Env;