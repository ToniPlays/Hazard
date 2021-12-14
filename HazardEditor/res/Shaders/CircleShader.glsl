#type Vertex
#version 450

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_LocalPosition;
layout(location = 2) in vec4 v_Color;
layout(location = 3) in float v_Thickness;
layout(location = 4) in float v_Fade;

layout(location = 0) out vec4 f_Color;
layout(location = 1) out vec3 f_LocalPosition;
layout(location = 2) out float f_Thickness;
layout(location = 3) out float f_Fade;

layout(std140, binding = 0) uniform Camera {
	uniform mat4 u_ViewProjection;
} u_Camera;

void main() 
{
	f_Color = v_Color;
	f_LocalPosition = v_LocalPosition;
	f_Thickness = v_Thickness;
	f_Fade = v_Fade;
	gl_Position = u_Camera.u_ViewProjection * vec4(v_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 f_Color;
layout(location = 1) in vec3 f_LocalPosition;
layout(location = 2) in float f_Thickness;
layout(location = 3) in float f_Fade;

layout(location = 0) out vec4 color;

void main() 
{

	float dist = 1.0 - length(f_LocalPosition);
	float circle = smoothstep(0.0, f_Fade, dist);

	color = vec4(dist, dist, dist, 1.0);
	circle *= smoothstep(f_Thickness + f_Fade, f_Thickness, dist);

	if(circle == 0.0) discard;
	color = f_Color;
	color.a *= circle;
}
