#version 450 core

in vec2 f_coords;
in vec2 blurCoords[11];

uniform sampler2D sampleTexture;

out vec4 color;

void main() {

	color = vec4(0.0);

	color += texture(sampleTexture, blurCoords[0]) * 0.0093;
	color += texture(sampleTexture, blurCoords[1]) * 0.028002;
	color += texture(sampleTexture, blurCoords[2]) * 0.065984;
	color += texture(sampleTexture, blurCoords[3]) * 0.121703;
	color += texture(sampleTexture, blurCoords[4]) * 0.175713;
	color += texture(sampleTexture, blurCoords[5]) * 0.198596;
	color += texture(sampleTexture, blurCoords[6]) * 0.175713;
	color += texture(sampleTexture, blurCoords[7]) * 0.121703;
	color += texture(sampleTexture, blurCoords[8]) * 0.065984;
	color += texture(sampleTexture, blurCoords[9]) * 0.028002;
	color += texture(sampleTexture, blurCoords[10]) * 0.0093;

}