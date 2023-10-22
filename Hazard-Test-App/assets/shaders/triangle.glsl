
#type Vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

layout(location = 0) out vec4 Color;

void main() 
{
	Color = a_Color;
	gl_Position = vec4(a_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 Color;

layout(location = 0) out vec4 OutputColor;

void main() 
{
	OutputColor = Color;
}