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
#include "Utils/Common.glsl"
#include "Utils/Lighting.glsl"
#include "Utils/PostProcessing.glsl"

layout(location = 0) in vec4 f_Color;
layout(location = 1) in vec3 FragPos;
layout(location = 2) in vec3 f_Normal;

layout(binding = 0) uniform samplerCube u_IrradianceMap;
layout(binding = 1) uniform samplerCube u_PrefilterMap;
layout(binding = 2) uniform sampler2D u_BRDFLut;

layout(location = 0) out vec4 OutputColor;


const float gamma = 2.2;

const float metallic = 1.0;
const float roughness = 0.1;


void main() 
{
	float ao = u_Lights.SkyLightIntensity;
	vec3 albedo = f_Color.rgb;


	vec3 N = normalize(f_Normal);
	vec3 V = normalize(u_Camera.Position.xyz - FragPos);
	vec3 R = reflect(V, N);

	vec3 F0 = mix(vec3(0.04), albedo, metallic);


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

		float NDF	= DistributionGGX(N, H, roughness);
		float G		= GeometrySmith(N, V, L, roughness);
		vec3 F		= FresnelSchlick(max(dot(H, V), 0.0), F0);
		
		vec3 numerator		= NDF * G * F;
		float denominator	= 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
		vec3 specular = numerator / denominator;

		//kS is equal to Fresnel
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		
		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);

		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}
	
	//Ambient lighting

	vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;
	
	vec3 irradiance = texture(u_IrradianceMap, N).rgb;
	vec3 diffuse = irradiance * albedo;

	//Sample both the pre-filter map and the BRDF lut
	const float MAX_REFLECTION_LOD = 1.0;

	vec3 prefilteredColor	= textureLod(u_PrefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
	vec2 brdf				= texture(u_BRDFLut, vec2(max(dot(-N, V), 0.0), roughness)).rg;
	vec3 specular			= prefilteredColor * (F * brdf.x + brdf.y);

	vec3 ambient			= (kD * diffuse + specular) * ao;

	vec3 color				= ambient + Lo;
	

	//Tonemapping
	color = ACESTonemap(color);
	color = GammaCorrect(color, gamma);

	OutputColor = vec4(color, 1.0);
}
