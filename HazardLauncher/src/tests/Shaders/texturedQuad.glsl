
#type Vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextureCoords;

layout(location = 0) out vec4 Color;
layout(location = 1) out vec2 TextureCoords;

void main() 
{
	Color = a_Color;
	TextureCoords = a_TextureCoords;
	gl_Position = vec4(a_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 Color;
layout(location = 1) in vec2 TextureCoords;

layout(binding = 0) uniform sampler2D u_Texture;

layout(location = 0) out vec4 OutputColor;

void main() 
{
	OutputColor = texture(u_Texture, TextureCoords) * Color;
}
