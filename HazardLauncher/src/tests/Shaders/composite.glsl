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

vec3 ACESTonemap(vec3 color)
{
	mat3 m1 = mat3(
		0.59719, 0.07600, 0.02840,
		0.35458, 0.90834, 0.13383,
		0.04823, 0.01566, 0.83777
	);
	mat3 m2 = mat3(
		1.60475, -0.10208, -0.00327,
		-0.53108, 1.10813, -0.07276,
		-0.07367, -0.00605, 1.07602
	);
	vec3 v = m1 * color;
	vec3 a = v * (v + 0.0245786) - 0.000090537;
	vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
	return clamp(m2 * (a / b), 0.0, 1.0);
}
vec3 GammaCorrect(vec3 color, float gamma)
{
	return pow(color, vec3(1.0 / gamma));
}


void main() 
{
	vec3 color = texture(u_Image, v_TexCoords).rgb;
	color = ACESTonemap(color);
	color = GammaCorrect(color, 2.2);
	OutputColor = vec4(color, 1.0);
}