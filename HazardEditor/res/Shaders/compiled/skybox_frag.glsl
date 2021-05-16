#version 330 core

in vec3 texCoords;

uniform samplerCube SkyboxCubemap;
uniform float gamma;

out vec4 color;

vec4 mapHDR(vec3 color) {

	vec3 mapped = color / (color + vec3(1.0));
	mapped = pow(mapped, vec3(1.0 / gamma));

	return vec4(mapped, 1.0);
}

void main() {
	color = mapHDR(texture(SkyboxCubemap, texCoords).rgb);
}