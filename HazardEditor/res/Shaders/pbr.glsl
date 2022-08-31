#type Vertex
#version 450

#include "Uniforms/CameraUniform.glsl"
#include "Uniforms/ModelUniform.glsl"
#include "Inputs/InputPBR.glsl"

layout(location = 0) out vec4 f_Color;
layout(location = 1) out vec3 FragPos;
layout(location = 2) out vec3 f_Normal;

void main() 
{
	f_Color = a_Color;
	f_Normal = mat3(transpose(inverse(u_Model.u_Transform))) * a_Normal, 1.0;

	vec4 worldPosition = u_Model.u_Transform * vec4(a_Position, 1.0);

	FragPos = worldPosition.xyz;
	gl_Position = u_Camera.ViewProjection * worldPosition;
}

#type Fragment
#version 450

#include "Uniforms/CameraUniform.glsl"
#include "Uniforms/LightSources.glsl"

layout(location = 0) in vec4 f_Color;
layout(location = 1) in vec3 FragPos;
layout(location = 2) in vec3 f_Normal;

layout(location = 0) out vec4 color;



const float ambientStrength = 0.2;
const float gamma = 1.1;
const float specularStrength = 0.3;

vec3 GammaCorrection(vec4 color) 
{
	return pow(color.rgb, vec3(1.0 / gamma));
}

const vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
const vec3 lightDir = vec3(1.0, 1.0, 1.0);

void main() 
{
	//DirectionalLight light = u_Lights.u_DirectionalLights[0];

	float diffuseStrength = max(dot(normalize(f_Normal), normalize(lightDir)), 0.0);
	vec3 diffuse = diffuseStrength * lightColor.rgb;
	vec4 ambientColor = ambientStrength * vec4(lightColor.rgb, 1.0);

	color = (ambientColor + vec4(diffuse, 1.0)) * vec4(f_Color.rgb, 1.0);

	//color = vec4(GammaCorrection(color), 1.0);

}
