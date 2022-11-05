#type Vertex
#version 450 core

layout(location = 0) out vec2 v_TexCoords;

vec3 quadPos[6] = vec3[](
	vec3( 1,  1, 0), vec3(-1, -1, 0), vec3(-1,  1, 0),
	vec3(-1, -1, 0), vec3( 1,  1, 0), vec3( 1, -1, 0)
);

void main() 
{
	vec3 pos = quadPos[gl_VertexIndex];
	gl_Position = vec4(pos, 1.0);
	v_TexCoords = vec2((pos.x + 1.0) * 0.5, (pos.y + 1.0) * 0.5);
}

#type Fragment
#version 450 core

layout(location = 0) in vec2 v_TexCoords;

layout (binding = 0) uniform sampler2D u_Image;

layout(location = 0) out vec4 OutputColor;

void main() 
{
	vec3 color = texture(u_Image, v_TexCoords).rgb;
	OutputColor = vec4(color, 1.0);
}