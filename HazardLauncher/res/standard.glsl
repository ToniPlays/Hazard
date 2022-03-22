#type Vertex
#version 450

layout(location = 0) in vec3 v_Position;

layout(location = 0) out vec4 f_Position;

void main() 
{
	vec4 pos = vec4(v_Position, 1.0);
	f_Position = pos;
	gl_Position = pos;
}

#type Fragment
#version 450

layout(location = 0) in vec4 f_Position;

layout(location = 0) out vec4 color;

void main() 
{
	color = f_Position;
}
