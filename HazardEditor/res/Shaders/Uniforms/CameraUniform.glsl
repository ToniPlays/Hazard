layout(std140, binding = 0) uniform Camera {
	uniform mat4 u_ViewProjection;
	uniform mat4 u_Projection;
	uniform mat4 u_View;
	uniform vec4 u_Position;
} u_Camera;