#ifdef VULKAN_API
layout(std140, set = 0, binding = 0) uniform Camera {
#else
layout(std140, binding = 0) uniform Camera {
#endif
	uniform mat4 ViewProjection;
	uniform mat4 Projection;
	uniform mat4 View;
	uniform mat4 InverseViewProjection;
	uniform vec4 Position;
	uniform vec4 Unused1;
	uniform vec4 Unused2;
	uniform vec4 Unused3;
} u_Camera;