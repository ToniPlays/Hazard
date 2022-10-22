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
#include "Utils/Common.glsl"
#include "Utils/Lighting.glsl"

layout(location = 0) in vec4 f_Color;
layout(location = 1) in vec3 FragPos;
layout(location = 2) in vec3 f_Normal;

layout(binding = 0) uniform samplerCube u_EnvironmentMap;
layout(binding = 1) uniform samplerCube u_Irradiance;

layout(location = 0) out vec4 OutputColor;


const float gamma = 2.2;

const float metallic = 0.5;
const float roughness = 0.3;


void main() 
{
	vec3 albedo = f_Color.rgb;
	float ao = u_Lights.SkyLightIntensity;

	vec3 N = normalize(f_Normal);
	vec3 V = normalize(u_Camera.Position.xyz - FragPos);
	vec3 R = reflect(V, N);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	vec3 Lo = vec3(0.0);

	//Calculate lights
	for(int i = 0; i < 1; ++i)
	{
		//Lighting
		DirectionalLight light = u_Lights.DirectionalLights[i];

		vec3 L = light.Direction.xyz;
		vec3 H = normalize(V + L);

		vec3 radiance = light.Color.rgb * light.Color.a;
		//-------

		float NDF	= DistributionGGX(N, H, roughness);
		float G		= GeometrySmith(N, V, L, roughness);
		vec3 F		= FreshnelSchlick(max(dot(H, V), 0.0), F0);
		
		vec3 numerator		= NDF * G * F;
		float denominator	= 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;

		vec3 specular = numerator / denominator;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		
		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);

		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}
	
	vec3 kS = FreshnelSchlick(max(dot(N, V), 0.0), F0);
	vec3 kD = 1.0 - kS;
	kD *= metallic;
	
	vec3 irradiance = texture(u_EnvironmentMap, R).rgb;
	vec3 diffuse = irradiance * albedo;
	vec3 ambient = (kD * diffuse) * ao;

	vec3 color = ambient + Lo;
	
	//Tonemapping
	color = color / (color + vec3(1.0));

	//Gamma correct
	color = pow(color, vec3(1.0 / gamma));

	OutputColor = vec4(color, 1.0);
}
