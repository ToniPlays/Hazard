#type Vertex
#version 450

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec4 v_Color;
layout(location = 2) in vec2 v_TextureCoords;
layout(location = 3) in float v_TextureIndex;

layout(location = 0) out vec4 f_Color;
layout(location = 1) out vec2 f_TextureCoords;
layout(location = 2) out float f_TextureIndex;

layout(std140, binding = 0) uniform Camera {
	uniform mat4 u_ViewProjection;
	uniform mat4 u_Projection;
	uniform mat4 u_View;
	uniform vec4 u_Position;
} u_Camera;

void main() 
{
	f_Color = v_Color;
	f_TextureCoords = v_TextureCoords;
	f_TextureIndex = v_TextureIndex;

	gl_Position = u_Camera.u_ViewProjection * vec4(v_Position, 1.0);
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
	
	vec4 textureColor = texture(u_Textures[index], f_TextureCoords);
	
	color = textureColor * f_Color;
}
