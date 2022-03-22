#type Vertex
#version 450
layout(location = 0) in vec3 v_ScreenPos;
layout(location = 1) in vec4 v_Color;
layout(location = 2) in float v_TexCoord;

layout(location = 0) out vec4 f_Color;
layout(location = 1) out float f_TexCoord;

void main() 
{
	f_Color = v_Color;
	f_TexCoord = v_TexCoord;
	gl_Position = vec4(v_ScreenPos, 1.0);
}
#type Fragment
#version 450

layout(location = 0) in flat vec4 f_Color;
layout(location = 1) in flat float f_TexCoord;

layout(location = 0) out vec4 color;

void main() 
{
	color = f_Color;
}