#version 450 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoords;

layout(std140, binding = 1) uniform PostPro 
{
	vec2 resolution;
	float v_outer;
	float v_inner;
	float v_intensity;
};

out vec2 f_coords;
out vec2 blurCoords[11];

void main()
{
	f_coords = texCoords;
	gl_Position = vec4(pos, 1.0, 1.0);

	vec2 center = pos.xy * 0.5 + 0.5;
	float pixelSize = 1.0 / (resolution.x * 0.6);

	for(int i = -5; i < 5; i++) {
		blurCoords[i + 5] = center + vec2(0.0, pixelSize * i);
	}
}