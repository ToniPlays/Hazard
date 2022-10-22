#ifdef VULKAN_API
layout(std140, set = 0, binding = 2) uniform Camera {
#else
layout(std140, binding = 2) uniform Camera {
#endif
	uniform mat4 ViewProjection;
	uniform mat4 Projection;
	uniform mat4 View;
	uniform mat4 InverseViewProjection;
	uniform vec4 Position;
	uniform float ZNear;
	uniform float ZFar;
	uniform vec2 Unused1;
	uniform vec4 Unused2;
	uniform vec4 Unused3;
} u_Camera;
