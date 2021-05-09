#type Vertex
#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec2 v_texCoords;

uniform mat4 viewProjection;
uniform mat4 model;
uniform vec3 cameraPos;

out vec4 f_color;
out vec3 f_normal;
out vec2 texCoords;
out vec3 reflectedVector;

void main() {


	vec4 worldPos = model * vec4(v_position, 1.0);
	gl_Position = viewProjection * worldPos;
	f_color = v_color;
	f_normal = v_normal;
	texCoords = v_texCoords;

	vec3 unitNormal = normalize(v_normal);
	vec3 viewVector = normalize(worldPos.xyz - cameraPos);

	reflectedVector = reflect(viewVector, unitNormal);
}

#type Fragment
#version 330 core

in vec4 f_color;
in vec3 f_normal;
in vec2 texCoords;
in vec3 reflectedVector;

uniform samplerCube envMap;

out vec4 color;

const float gamma = 0.6;

vec4 mapHDR(vec3 color) {
	vec3 mapped = color / (color + vec3(1.0));

	mapped = pow(mapped, vec3(1.0 / gamma));

	return vec4(mapped, 1.0);
}

void main() 
{
	vec4 reflectedColor = texture(envMap, reflectedVector);

	color = mapHDR(reflectedColor.rgb);
}