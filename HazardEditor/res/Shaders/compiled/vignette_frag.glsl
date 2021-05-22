#version 450 core

in vec2 f_coords;

uniform sampler2D sampleTexture;
uniform vec2 resolution;

out vec4 color;

void main() {

	vec4 c = texture(sampleTexture, f_coords);

	vec2 relativePos = gl_FragCoord.xy / resolution - 0.5;
	float len = length(relativePos);
	float vignette = smoothstep(0.6, 0.3, len);
	color = vec4(mix(c.rgb, c.rgb * vignette, 0.4), 1.0);

}