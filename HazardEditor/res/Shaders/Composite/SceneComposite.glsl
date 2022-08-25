#type Vertex
#version 450 core

layout(location = 0) out vec2 v_TexCoords;


vec3 quadPos[6] = vec3[](
	vec3( 1,  1, 0), vec3(-1, -1, 0), vec3(-1,  1, 0),
	vec3(-1, -1, 0), vec3( 1,  1, 0), vec3( 1, -1, 0)
);

void main() 
{
	vec3 pos = quadPos[gl_VertexIndex];
	gl_Position = vec4(pos, 1.0);

	v_TexCoords = vec2((pos.x + 1.0) * 0.5, (pos.y + 1.0) * 0.5);
}

#type Fragment
#version 450 core

#include "../Uniforms/CameraUniform.glsl"
#include "../Uniforms/LightSources.glsl"
#include "../Utils/Lighting.glsl"

layout (binding = 0) uniform sampler2D gPosition;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D gAlbedoSpec;

layout(location = 0) in vec2 v_TexCoords;

layout(location = 0) out vec4 color;

const float ambientStrength = 0.15;
const float gamma = 1.1;
const float specularStrength = 0.25;

vec3 GammaCorrection(vec4 color) {
	return pow(color.rgb, vec3(1.0 / gamma));
}

void main() 
{
	
	vec3 fragPos = texture(gPosition, v_TexCoords).rgb;
	vec3 Normal = texture(gNormal, v_TexCoords).rgb;
	vec4 Albedo = texture(gAlbedoSpec, v_TexCoords);

	DirectionalLight light = u_Lights.u_DirectionalLights[0];

	float diffuseStrength = max(dot(Normal, light.Direction.xyz), 0.0);
	vec3 diffuse = diffuseStrength * light.Color.rgb;
	vec4 ambientColor = ambientStrength * vec4(light.Color.rgb, 1.0);

	color = (ambientColor + vec4(diffuse, 1.0)) * vec4(Albedo.rgb, 1.0);

	//color = vec4(GammaCorrection(color), 1.0);
}