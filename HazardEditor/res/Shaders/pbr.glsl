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

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

void main() 
{
	gPosition = vec4(FragPos, 1.0);
	gNormal = vec4(normalize(f_Normal), 1.0);
	gAlbedoSpec = f_Color;
}
