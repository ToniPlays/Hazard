#type Vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in float a_TextureIndex;

layout(location = 0) out vec4 f_Color;
layout(location = 1) out vec2 f_TextureCoords;
layout(location = 2) out float f_TextureIndex;

layout(std140, binding = 0) uniform Camera {
	uniform mat4 u_ViewProjection;
	uniform mat4 u_Projection;
	uniform mat4 u_View;
	uniform vec4 u_Position;
} u_Camera;


layout(std140, binding = 1) uniform Model {
	uniform mat4 u_Transform;
} u_Model;

void main() 
{
	f_Color = a_Color;
	f_TextureCoords = vec2(0.0, 0.0);
	f_TextureIndex = a_TextureIndex;

	gl_Position = u_Camera.u_ViewProjection * vec4(a_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 f_Color;
layout(location = 1) in vec2 f_TextureCoords;
layout(location = 2) in flat float f_TextureIndex;

layout (binding = 1) uniform sampler2D u_Textures[32];

layout(location = 0) out vec4 color;


void main() 
{

	int index = int(f_TextureIndex);
	
	vec4 textureColor = vec4(1.0, 1.0, 1.0, 1.0);
	color = textureColor * f_Color;
}
