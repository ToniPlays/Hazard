#type Vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;
uniform vec3 lightPos;

out vec3 surfaceNormal;
out vec3 lightVec;
out vec2 passCoords;

void main() {

	vec4 worldPos = transform * vec4(position, 1.0);
	
	surfaceNormal = (transform * vec4(normal, 0.0)).xyz;
	lightVec = lightPos - worldPos.xyz;

	gl_Position = projection * view * worldPos;
	passCoords = textureCoords;
}

#type Fragment
#version 330 core

in vec3 surfaceNormal;
in vec2 passCoords;
in vec3 lightVec;

uniform float test;
uniform vec3 lightColor;
uniform sampler2D T_texture;

out vec4 color;

void main() {
	
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLight = normalize(lightVec);

	float nDotl = dot(unitNormal, unitLight);
	float brightness = max(nDotl, 0.5);
	vec3 diffuse = brightness * lightColor;
	color = vec4(diffuse, 1.0) * vec4(1.0, 1.0, 1.0, 1.0);
}