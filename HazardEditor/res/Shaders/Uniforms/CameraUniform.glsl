#ifdef VULKAN_API
layout(std140, set = 0, binding = 2) uniform Camera {
#else
layout(std140, binding = 2) uniform Camera {
#endif
	uniform mat4 ViewProjection;
	uniform mat4 InverseViewProjection;
	uniform mat4 Projection;
	uniform mat4 InverseProjection;
	uniform mat4 View;
	uniform mat4 InverseView;
	uniform vec2 NDCToViewMul;
	uniform vec2 NDCToViewAdd;
	uniform vec2 DepthUnpack;
	uniform vec2 CameraTanHalfFOV;
	uniform vec4 Unused;
	uniform vec4 Unused2;
} u_Camera;
