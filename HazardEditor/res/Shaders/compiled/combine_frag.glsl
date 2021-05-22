#version 450 core

in vec2 f_coords;

uniform sampler2D[2] sampleTextures;

out vec4 color;

void main() {
	
	vec4 fColor = texture(sampleTextures[0], f_coords);
	vec4 sColor = texture(sampleTextures[1], f_coords);

	color = fColor + sColor;
}