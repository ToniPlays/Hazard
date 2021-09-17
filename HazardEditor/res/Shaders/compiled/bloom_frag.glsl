#version 450 core

in vec2 f_coords;

layout(std140, binding = 1) uniform PostPro 
{
	vec2 resolution;
	float v_outer;
	float v_inner;
	float v_intensity;
	float bloomThreshold;
	float bloomIntensity;
};

uniform sampler2D sampleTexture;

out vec4 color;

void main() {

	vec4 c = texture(sampleTexture, f_coords);

	float brightness = (c.r + c.g + c.b) / 3.0;

	if(brightness >= bloomThreshold)
		color = c * bloomIntensity;
	else 
		color = vec4(0.0);
}