#version 450 core

in vec2 f_coords;

layout(std140, binding = 1) uniform PostPro 
{
	vec2 resolution;
	float v_outer;
	float v_inner;
	float v_intensity;
};

uniform sampler2D sampleTexture;

out vec4 color;

void main() {

	vec4 c = texture(sampleTexture, f_coords);

	vec2 relativePos = gl_FragCoord.xy / resolution - 0.5;
	float len = length(relativePos);
	float vignette = smoothstep(v_outer, v_inner, len);
	color = vec4(mix(c.rgb, c.rgb * vignette, v_intensity), 1.0);

}