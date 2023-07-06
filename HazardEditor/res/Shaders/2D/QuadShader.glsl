#type Vertex
#version 450

#include "../Uniforms/CameraUniform.glslh"
#include "../Inputs/InputQuad.glslh"

layout(location = 0) out vec4 Color;
layout(location = 1) out vec2 TextureCoords;
layout(location = 2) out float TextureIndex;

layout(push_constant) uniform Transform
{
    mat4 Transform;
} p_Transform;

void main() 
{
	Color = a_Color;
	TextureCoords = a_TextureCoords;
	TextureIndex = a_TextureIndex;
	gl_Position = u_Camera.ViewProjection * vec4(a_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 Color;
layout(location = 1) in vec2 TextureCoords;
layout(location = 2) in flat float TextureIndex;

layout(location = 0) out vec4 color;

void main() 
{
	int index = int(TextureIndex);
	vec4 textureColor = vec4(1.0);
	color = textureColor * Color;
}
