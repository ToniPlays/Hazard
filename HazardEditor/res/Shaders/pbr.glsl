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

const float roughness = 0.0;

vec3 IBL(vec3 F0, vec3 Lr)
{
	vec3 irradiance = texture(u_IrradianceMap, m_Params.Normal).rgb;
	vec3 F = FresnelSchlickRoughness(m_Params.NdotV, F0, m_Params.Roughness);
	vec3 kd = (1.0 - F) * (1.0 - m_Params.Metalness);
	vec3 diffuseIBL = m_Params.Albedo * irradiance;


	float NoV = clamp(m_Params.NdotV, 0.0, 1.0);
	vec3 R = 2.0f * dot(m_Params.View, m_Params.Normal) * m_Params.Normal - m_Params.View;
	vec3 specularIrradiance	= textureLod(u_RadianceMap, Lr, m_Params.Roughness).rgb;

	//Sample BRDF
	vec2 specularBRDF				= texture(u_BRDFLut, vec2(m_Params.NdotV, m_Params.Roughness)).rg;
	vec3 specularIBL				= specularIrradiance * (F0 * specularBRDF.x + specularBRDF.y);

	return kd * diffuseIBL + specularIBL;
}

void main() 
{
	vec3 albedo = f_Color.rgb;

	vec3 N = normalize(f_Normal);
	vec3 V = normalize(u_Camera.Position.xyz - FragPos);
	vec3 R = reflect(-V, N);

	
	m_Params.Albedo = albedo;
	m_Params.Normal = N;
	m_Params.Metalness = (sin(u_Util.Time) + 1.0) / 2.0;
	m_Params.Roughness = roughness;
	m_Params.View = V;
	m_Params.NdotV = max(dot(N, V), 0.0);


	vec3 F0 = mix(vec3(0.04), m_Params.Albedo, m_Params.Metalness);
	vec3 Lo = vec3(0.0);

	//Reflectance
	for(int i = 0; i < 1; i++)
	{
		//Lighting
		DirectionalLight light = u_Lights.DirectionalLights[i];

		vec3 L = light.Direction.xyz;
		vec3 H = normalize(V + L);

		vec3 radiance = light.Color.rgb * light.Color.a;
		//-------
		float NdotL = max(dot(N, L), 0.0);

		float NDF	= DistributionGGX(N, H, roughness);
		float G		= GeometrySmith(N, V, L, roughness);
		vec3 F		= FresnelSchlick(max(dot(H, V), 0.0), F0);
		
		vec3 numerator		= NDF * G * F;
		float denominator	= 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
		vec3 specular = numerator / denominator;

		//kS is equal to Fresnel
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		
		kD *= 1.0 - m_Params.Metalness;

		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 Lr = 2.0 * m_Params.NdotV * m_Params.Normal - m_Params.View;
	vec3 ibl = IBL(F0, Lr);
	
	//Tonemapping
	vec3 color = ACESTonemap(ibl + Lo);
	color = GammaCorrect(color, gamma);

	OutputColor = vec4(color, 1.0);
}
