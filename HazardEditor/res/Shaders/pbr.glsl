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
	f_Normal = mat3(u_Model.Transform) * a_Normal;
	vec4 worldPosition = u_Model.Transform * vec4(a_Position, 1.0);

	FragPos = worldPosition.xyz;
	f_Color = a_Color;
	gl_Position = u_Camera.ViewProjection * worldPosition;
}

#type Fragment
#version 450

#include "Uniforms/CameraUniform.glsl"
#include "Uniforms/LightSources.glsl"
#include "Uniforms/ModelUniform.glsl"
#include "Uniforms/UtilityUniform.glsl"

layout(location = 0) in vec4 f_Color;
layout(location = 1) in vec3 FragPos;
layout(location = 2) in vec3 f_Normal;

layout(binding = 0) uniform samplerCube u_RadianceMap;
layout(binding = 1) uniform samplerCube u_IrradianceMap;
layout(binding = 2) uniform samplerCube u_PrefilterMap;
layout(binding = 3) uniform sampler2D u_BRDFLut;


#include "Utils/Common.glsl"
#include "Utils/Lighting.glsl"
#include "Utils/PostProcessing.glsl"

layout(location = 0) out vec4 OutputColor;


const float gamma = 2.2;

void main() 
{
	//Initialize Params
	m_Params.Albedo = f_Color.rgb;
	m_Params.Normal = normalize(f_Normal);
	m_Params.Metalness = u_Model.Metalness;
	m_Params.Roughness = u_Model.Roughness;
	m_Params.View = normalize(u_Camera.Position.xyz - FragPos);
	m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0);

	//Calculate
	vec3 F0 = mix(vec3(0.04), m_Params.Albedo, m_Params.Metalness);

	vec3 Lo = vec3(0.0);
	//Reflectance
	for(int i = 0; i < 1; i++)
	{
		//Lighting
		DirectionalLight light = u_Lights.DirectionalLights[i];
		if(light.Color.a == 0.0) 
			continue;

		Lo += CalculateDirectionalLight(F0, light);
	}

	vec3 Lr = 2.0 * m_Params.NdotV * m_Params.Normal - m_Params.View;
	vec3 ibl = IBL(F0, Lr) * u_Lights.SkyLightIntensity * u_Lights.IBLContribution;
	
	//Tonemapping
	vec3 color = ACESTonemap(ibl + Lo);
	color = GammaCorrect(color, gamma);

	OutputColor = vec4(color, 1.0);
}
