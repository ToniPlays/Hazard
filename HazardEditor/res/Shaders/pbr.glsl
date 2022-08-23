#type Vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec2 a_TextureCoords;

layout(std140, binding = 0) uniform Camera {
	uniform mat4 u_ViewProjection;
	uniform mat4 u_Projection;
	uniform mat4 u_View;
	uniform vec4 u_Position;
} u_Camera;

layout(location = 0) out vec4 f_Color;
layout(location = 1) out vec3 FragPos;
layout(location = 2) out vec3 f_Normal;

layout(std140, binding = 1) uniform Model {
	uniform mat4 u_Transform;
} u_Model;

void main() 
{
	f_Color = a_Color;
	f_Normal = mat3(transpose(inverse(u_Model.u_Transform))) * a_Normal, 1.0;

	vec4 worldPosition = u_Model.u_Transform * vec4(a_Position, 1.0);
	FragPos = worldPosition.xyz;

	gl_Position = u_Camera.u_ViewProjection * worldPosition;
}

#type Fragment
#version 450

layout(location = 0) in vec4 f_Color;
layout(location = 1) in vec3 FragPos;
layout(location = 2) in vec3 f_Normal;

layout(std140, binding = 0) uniform Camera {
	uniform mat4 u_ViewProjection;
	uniform mat4 u_Projection;
	uniform mat4 u_View;
	uniform vec4 u_Position;
} u_Camera;


layout(location = 0) out vec4 color;

const vec3 lightPos = { 100, 30, 100 };
const vec3 lightColor = { 1.0, 1.0, 1.0 };

const float ambientStrength = 0.15;
const float gamma = 2.2;
const float specularStrength = 0.25;

vec3 GammaCorrection(vec4 color) {
	return pow(color.rgb, vec3(1.0 / gamma));
}

void main() 
{
	vec3 norm = normalize(f_Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diffuseStrength = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStrength * lightColor;


	vec4 ambientColor = ambientStrength * vec4(lightColor, 1.0);

	color = (ambientColor + vec4(diffuse, 1.0f)) * f_Color;
}
