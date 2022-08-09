#type Vertex
#version 450
layout(location = 0) in vec3 a_ScreenPos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in float a_TexCoord;

layout(location = 0) out vec4 f_Color;
layout(location = 1) out float f_TexCoord;

void main() 
{
	f_Color = a_Color;
	f_TexCoord = a_TexCoord;
	gl_Position = vec4(a_ScreenPos, 1.0);
}
#type Fragment
#version 450

layout(location = 0) in vec4 f_Color;
layout(location = 1) in flat float f_TexCoord;

layout(location = 0) out vec4 color;

void main() 
{
	color = f_Color;
}