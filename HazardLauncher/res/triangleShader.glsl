#type Vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

layout(location = 0) out vec4 f_Color;

void main() 
{
	vec4 pos = vec4(a_Position, 1.0);
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
