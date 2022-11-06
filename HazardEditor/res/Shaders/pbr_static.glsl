#type Vertex
#version 450

#include "Uniforms/CameraUniform.glsl"
#include "Uniforms/ModelUniform.glsl"
#include "Inputs/InputPBR.glsl"

layout(location = 0) out vec4 Color;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec3 WorldPosition;

void main()
{
	vec4 worldPosition = u_Model.Transform * vec4(a_Position.xyz, 1.0);
	gl_Position = u_Camera.ViewProjection * worldPosition;

	Color = a_Color;
	Normal = mat3(u_Model.Transform) * a_Normal.xyz;
	WorldPosition = worldPosition.xyz;
	
}

#type Fragment
#version 450

#include "Uniforms/CameraUniform.glsl"
#include "Uniforms/LightSources.glsl"
#include "Uniforms/ModelUniform.glsl"
#include "Uniforms/UtilityUniform.glsl"

layout(location = 0) in vec4 Color;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 WorldPosition;

layout(set = 1, binding = 1) uniform samplerCube u_RadianceMap;

layout(set = 1, binding = 2) uniform samplerCube u_IrradianceMap;
layout(set = 1, binding = 3) uniform sampler2D u_BRDFLut;


#include "Utils/Common.glsl"
#include "Utils/Lighting.glsl"
#include "Utils/PostProcessing.glsl"

layout(location = 0) out vec4 OutputColor;

const float gamma = 2.2;

void main() 
{
	if((u_Util.Flags & (1 << 0)) > 0) 
	{
		OutputColor = vec4(1.0, 0.0, 0.0, 0.2);
	}
	else
	{
		//Initialize Params
		m_Params.Albedo = Color.rgb;
		m_Params.Metalness = u_Model.Metalness;
		m_Params.Roughness = max(u_Model.Roughness, 0.05);
		m_Params.Normal = normalize(Normal);
		m_Params.View = normalize(u_Util.CameraPos.xyz - WorldPosition);
		m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0);
	
		float alpha = Color.a;

		//Specular reflect direction
		vec3 Lr = 2.0 * m_Params.NdotV * m_Params.Normal - m_Params.View;
		//Dielectric
		vec3 F0 = mix(vec3(0.04), m_Params.Albedo, m_Params.Metalness);

		vec3 Lo = vec3(0.0);
		//Reflectance
		for(int i = 0; i < u_Lights.DirectionalLightCount; i++)
		{
			//Lighting
			DirectionalLight light = u_Lights.DirectionalLights[i];
			if(light.Color.a <= 0.0) 
				continue;
			Lo += CalculateDirectionalLight(F0, light);
		}

		vec3 ibl = IBL(F0, Lr) * u_Lights.SkyLightIntensity;
	
		//Tonemapping
		vec3 color = ACESTonemap(ibl + Lo);
		color = GammaCorrect(color, gamma);
	
		OutputColor = vec4(color, 1.0);
	}
}
