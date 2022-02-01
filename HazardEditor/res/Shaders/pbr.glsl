#type Vertex
#version 450

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec4 v_Color;
layout(location = 2) in vec3 v_Normal;
layout(location = 3) in vec2 v_TextureCoords;

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
	f_Color = v_Color;
	f_Normal = (u_Model.u_Transform * vec4(v_Normal, 1.0)).xyz;

	vec4 worldPosition = u_Model.u_Transform * vec4(v_Position, 1.0);
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
const vec4 lightColor = { 1.0, 1.0, 1.0, 1.0 };

const float ambient = 0.15;
const float gamma = 2.2;
const float specularStrength = 0.25;

vec3 GammaCorrection(vec4 color) {
	return pow(color.rgb, vec3(1.0 / gamma));
}

void main() 
{

	vec3 norm = normalize(f_Normal);
	vec3 lPos = normalize(lightPos);
	vec3 lightDir = normalize(lightPos - FragPos);

	float lightDot = dot(norm, lPos);

	//Diffuse
	float diffuse = max(lightDot, ambient);
	vec4 diffuseColor = diffuse * f_Color;

	//Specular
	vec3 viewDir = normalize(u_Camera.u_Position.xyz - FragPos);
	vec3 reflected = reflect(-lightDir, norm);
	float specular = pow(max(dot(viewDir, reflected), 0.0), 16);
	vec3 specColor = specularStrength * specular * lightColor.rgb;

	vec3 outColor = GammaCorrection(f_Color) * diffuseColor.rgb;

	color = vec4(outColor + specColor, 1.0);
}
