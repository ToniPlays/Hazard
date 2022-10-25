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
	f_Normal = mat3(u_Model.u_Transform) * a_Normal;
	vec4 worldPosition = u_Model.u_Transform * vec4(a_Position, 1.0);

	FragPos = worldPosition.xyz;
	f_Color = a_Color;
	gl_Position = u_Camera.ViewProjection * worldPosition;
}

#type Fragment
#version 450

#include "Uniforms/CameraUniform.glsl"
#include "Uniforms/LightSources.glsl"
#include "Uniforms/UtilityUniform.glsl"
#include "Utils/Common.glsl"
#include "Utils/Lighting.glsl"
#include "Utils/PostProcessing.glsl"

layout(location = 0) in vec4 f_Color;
layout(location = 1) in vec3 FragPos;
layout(location = 2) in vec3 f_Normal;

layout(binding = 0) uniform samplerCube u_RadianceMap;
layout(binding = 1) uniform samplerCube u_IrradianceMap;
layout(binding = 2) uniform samplerCube u_PrefilterMap;
layout(binding = 3) uniform sampler2D u_BRDFLut;

layout(location = 0) out vec4 OutputColor;


const float gamma = 2.2;

vec3 IBL(vec3 F0, vec3 Lr)
{
	vec3 irradiance = texture(u_IrradianceMap, m_Params.Normal).rgb;
	vec3 F = FresnelSchlickRoughness(m_Params.NdotV, F0, m_Params.Roughness);
	vec3 kD = (1.0 - F) * (1.0 - m_Params.Metalness);
	vec3 diffuseIBL = m_Params.Albedo * irradiance;

	float NoV = clamp(m_Params.NdotV, 0.0, 1.0);
	vec3 R = 2.0 * dot(m_Params.View, m_Params.Normal) * m_Params.Normal - m_Params.View;
	vec3 specularIrradiance	= texture(u_RadianceMap, Lr).rgb;

	//Sample BRDF
	vec2 specularBRDF				= texture(u_BRDFLut, vec2(m_Params.NdotV, m_Params.Roughness)).rg;
	vec3 specularIBL				= specularIrradiance * (F0 * specularBRDF.x + specularBRDF.y);

	return kD * diffuseIBL + specularIBL;
}

void main() 
{
	m_Params.Albedo = f_Color.rgb;
	m_Params.Normal = normalize(f_Normal);
	m_Params.Metalness = clamp(u_Util.Time, 0.002, 0.998);
	m_Params.Roughness = clamp(1.0 - m_Params.Metalness, 0.002, 0.998);
	m_Params.View = normalize(u_Camera.Position.xyz - FragPos);
	m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0);


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
	vec3 ibl = IBL(F0, Lr) * u_Lights.IBLContribution * u_Lights.SkyLightIntensity;
	
	//Tonemapping
	vec3 color = ACESTonemap(ibl + Lo);
	color = GammaCorrect(color, gamma);

	OutputColor = vec4(color, 1.0);
}
