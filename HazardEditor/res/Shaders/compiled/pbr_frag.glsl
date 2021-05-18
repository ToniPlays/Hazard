#version 450 core

in vec4 f_color;
in vec3 f_normal;
in vec2 texCoords;
in vec3 reflectedVector;

uniform samplerCube envMap;
uniform float gamma;

out vec4 color;

vec4 mapHDR(vec3 color) {
	vec3 mapped = color / (color + vec3(1.0));
	mapped = pow(mapped, vec3(1.0 / gamma));
	return vec4(mapped, 1.0);
}

void main() 
{
	vec4 reflectedColor = texture(envMap, reflectedVector);
	color = mapHDR(reflectedColor.rgb) + vec4(0.2);
}