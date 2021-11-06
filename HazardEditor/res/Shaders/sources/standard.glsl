#type Vertex
#version 450

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec4 v_Color;
layout(location = 2) in vec2 v_TextureCoords;
layout(location = 3) in float v_TextureIndex;

layout(location = 0) out vec4 f_Color;
layout(location = 1) out vec2 f_TextureCoords;
layout(location = 2) out float f_TextureIndex;

void main() 
{
	f_Color = v_Color;
	f_TextureCoords = v_TextureCoords;
	f_TextureIndex = v_TextureIndex;

	gl_Position = vec4(v_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 f_Color;
layout(location = 1) in vec2 f_TextureCoords;
layout(location = 2) in float f_TextureIndex;

layout(location = 0) out vec4 color;

void main() 
{
	int index = int(f_TextureIndex);
	color = f_Color;
}
