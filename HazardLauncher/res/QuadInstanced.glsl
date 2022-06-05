#type Vertex
#version 450

layout(location = 0) in vec3 a_Position;

//Instanced data
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec4 a_MRow0;
layout(location = 3) in vec4 a_MRow1;
layout(location = 4) in vec4 a_MRow2;

layout(std140, binding = 0) uniform Camera {
	uniform mat4 ViewProjection;
} u_Camera;


layout(location = 0) out vec4 f_Color;

void main() 
{

	mat4 transform = mat4(
		vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
		vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
		vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
		vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
	);

	vec4 pos = u_Camera.ViewProjection * transform * vec4(a_Position, 1.0);
	f_Color = a_Color;
	gl_Position = pos;
}
 
#type Fragment
#version 450

layout(location = 0) in vec4 f_Color;

layout(location = 0) out vec4 color;

void main() 
{
	color = f_Color;
}
