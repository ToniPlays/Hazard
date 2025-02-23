#type Vertex
#version 450
#include "Uniforms/CameraUniform.glslh"



			layout(location = 0) in vec3 IN_Position;
			layout(location = 1) in vec4 IN_Color;
			layout(location = 2) in vec3 IN_Normal;
			layout(location = 3) in vec3 IN_Tangent;
			layout(location = 4) in vec3 IN_Binormal;
			layout(location = 5) in vec3 IN_TextureCoords;
			layout(location = 6) in vec4 IN_MRow0;
			layout(location = 7) in vec4 IN_MRow1;
			layout(location = 8) in vec4 IN_MRow2;
			

			layout(location = 0) out vec4 OUT_Color;
			layout(location = 1) out vec3 OUT_WorldPosition;
			layout(location = 2) out vec3 OUT_Normal;
			layout(location = 3) out vec2 OUT_TextureCoords;
			layout(location = 4) out mat3 OUT_WorldNormal;
			layout(location = 7) out flat uint OUT_EntityID;
			

			void main() 
			{
				mat4 transform = mat4(
					vec4(IN_MRow0.x, IN_MRow1.x, IN_MRow2.x, 0.0),
					vec4(IN_MRow0.y, IN_MRow1.y, IN_MRow2.y, 0.0),
					vec4(IN_MRow0.z, IN_MRow1.z, IN_MRow2.z, 0.0),
					vec4(IN_MRow0.w, IN_MRow1.w, IN_MRow2.w, 1.0)
				);

				vec4 worldPosition = transform * vec4(IN_Position, 1.0);
				gl_Position = u_Camera.ViewProjection * worldPosition;

				OUT_Color = IN_Color;
				OUT_Normal = mat3(transform) * IN_Normal;
				OUT_WorldNormal = mat3(transform) * mat3(IN_Tangent, IN_Binormal, IN_Normal);
				OUT_WorldPosition = worldPosition.xyz;
				OUT_TextureCoords = IN_TextureCoords.xy;
				
				OUT_EntityID = 0;
			}

#type Fragment
#version 450
#include "Uniforms/CameraUniform.glslh"
#include "Utils/Common.glslh"
#include "Utils/Lighting.glslh"
#include "Utils/PostProcessing.glslh"

layout (set = 0, binding = 1) uniform samplerCube u_RadianceMap;
layout (set = 0, binding = 2) uniform samplerCube u_IrradianceMap;
layout (set = 0, binding = 3) uniform sampler2D u_BRDFLut;
layout (set = 1, binding = 0) uniform sampler2D _Albedo;
layout (set = 1, binding = 1) uniform sampler2D _NormalMap;


layout(push_constant, std140) uniform PushConstants
{	uniform vec4 Albedo;
	uniform float Metalness;
	uniform float Roughness;
	uniform bool UseNormalMap;
	uniform bool Padding1;
	uniform bool Padding2;
	uniform bool Padding3;
} CONSTANT;
			layout(location = 0) in vec4 IN_Color;
			layout(location = 1) in vec3 IN_WorldPosition;
			layout(location = 2) in vec3 IN_Normal;
			layout(location = 3) in vec2 IN_TextureCoords;
			layout(location = 4) in mat3 IN_WorldNormal;
			layout(location = 7) in flat uint IN_EntityID;
			

			layout(location = 0) out vec4 OUT_Color;
			layout(location = 1) out uint OUT_EntityID;
			

			const float gamma = 2.2;
			const vec3 dielectric = vec3(0.04);

			vec3 IBL(vec3 F0, vec3 Lr, vec3 normal, vec3 albedo, float NDotV, float roughness, float metalness)
{
				vec3 irradiance = texture(u_IrradianceMap, normal).rgb;
				vec3 F = FresnelSchlickRoughness(F0, NDotV, roughness);
				vec3 kD = (1.0 - F) * (1.0 - metalness);
				vec3 diffuseIBL = albedo * irradiance;

				int radianceTexelLeves = textureQueryLevels(u_RadianceMap);
				vec3 specularIrradiance	= textureLod(u_RadianceMap, RotateVectorAboutY(1.0, Lr), roughness * radianceTexelLeves).rgb;

				//Sample BRDF
				vec2 specularBRDF				= texture(u_BRDFLut, vec2(NDotV, roughness)).rg;
				vec3 specularIBL				= specularIrradiance * (F0 * specularBRDF.x + specularBRDF.y);
				return kD * diffuseIBL + specularIBL;
			}

			void main() 
			{
				vec3 albedo = texture(_Albedo, IN_TextureCoords).rgb * CONSTANT.Albedo.rgb * IN_Color.rgb;
				float roughness = max(CONSTANT.Roughness, 0.05);
				vec3 normal = normalize(IN_Normal);

				if (CONSTANT.UseNormalMap)
				{
					normal = normalize(texture(_NormalMap, IN_TextureCoords).rgb * 2.0 - 1.0);
					normal = normalize(IN_WorldNormal * normal);
				}

				vec3 view = normalize(u_Camera.Position.xyz - IN_WorldPosition);
				float NdotV = max(dot(normal, view), 0.0);

				vec3 Lr = 2.0 * NdotV * normal - view;
				vec3 F0 = mix(dielectric, albedo, CONSTANT.Metalness);

				//Light calculations
				vec3 Lo = vec3(0.0);
				
				//Calculate final color
				vec3 ibl = IBL(F0, Lr, IN_Normal, albedo, NdotV, roughness, CONSTANT.Metalness);

				vec3 color = IN_Color.rgb;
				color = ACESTonemap(ibl + Lo);
				color = GammaCorrect(color, gamma);

				OUT_Color = vec4(color, 1.0);
				OUT_EntityID = IN_EntityID;
			}

