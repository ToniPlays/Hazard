#type Vertex
#version 330 core

layout(location = 0) in vec3 v_position;

uniform mat4 viewProjection;
out vec3 texCoords;

void main() {
	
	texCoords = v_position;
	vec4 pos = viewProjection * vec4(v_position, 1.0);
	gl_Position = pos.xyww;
}

#type Fragment
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