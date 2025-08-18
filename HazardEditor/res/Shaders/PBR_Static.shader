Shader "3D/Lit_Static"
{
	Language "GLSL"
	Type "3D/Lit"
	Version 450

	Depth "LessOrEqual"
	DepthWrite True

	Properties 
	{
		u_Camera ("Camera", CameraBlock, 0, 0, 1)
		u_RadianceMap ("RadianceMap", samplerCube, 0, 1, 1)
		u_IrradianceMap ("IrradianceMap", samplerCube, 0, 2, 1)
		u_BRDFLut ("BRDFLut", sampler2D, 0, 3, 1)
		_Albedo ("Albedo", sampler2D, 1, 0, 1)
		_NormalMap ("NormalMap", sampler2D, 1, 1, 1)
	}

	Constants 
	{
		Albedo ("Albedo", float4)
		Metalness ("Metalness", float)
		Roughness ("Roughness", float)
		UseNormalMap ("UseNormalMap", bool)
		Padding1 ("Padding1", bool)
		Padding2 ("Padding2", bool)
		Padding3 ("Padding3", bool)
	}

	Vertex
	{
		Include 
		{
			"Uniforms/CameraUniform.glslh"
		}

		Source 
		{			
			struct VS_In : VS_IN
			{
				vec3 Position;
				vec4 Color;
				vec3 Normal;
				vec3 Tangent;
				vec3 Binormal;
				vec3 TextureCoords;

				vec4 MRow0;
				vec4 MRow1;
				vec4 MRow2;
			};

			struct VS_Out : VS_OUT 
			{
				vec4 Color;
				vec3 WorldPosition;
				vec3 Normal;
				vec2 TextureCoords;
				mat3 WorldNormal;
				uint EntityID;
			};

			void main() 
			{
				mat4 transform = mat4(
					vec4(IN.MRow0.x, IN.MRow1.x, IN.MRow2.x, 0.0),
					vec4(IN.MRow0.y, IN.MRow1.y, IN.MRow2.y, 0.0),
					vec4(IN.MRow0.z, IN.MRow1.z, IN.MRow2.z, 0.0),
					vec4(IN.MRow0.w, IN.MRow1.w, IN.MRow2.w, 1.0)
				);
				
				gl_Position = OUT.Position;

				vec4 worldPosition = transform * vec4(IN.Position, 1.0);
				gl_Position =  u_Camera.ViewProjection * worldPosition;

				OUT.Color = IN.Color;
				OUT.Normal = mat3(transform) * IN.Normal;
				OUT.WorldNormal = mat3(transform) * mat3(IN.Tangent, IN.Binormal, IN.Normal);
				OUT.WorldPosition = worldPosition.xyz;
				OUT.TextureCoords = IN.TextureCoords.xy;
				
				OUT.EntityID = 0;
			}
		}
	}

	Fragment
	{
		Include
		{
			"Uniforms/CameraUniform.glslh",
			"Utils/Common.glslh",
			"Utils/Lighting.glslh",
			"Utils/PostProcessing.glslh"
		}

		Source
		{
			struct VS_Out : FS_IN 
			{
				vec4 Color;
				vec3 WorldPosition;
				vec3 Normal;
				vec2 TextureCoords;
				mat3 WorldNormal;
				flat uint EntityID;
			};

			struct FS_Out : FS_OUT
			{
				vec4 Color;
				uint EntityID;
			};

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
				vec3 albedo = texture(_Albedo, IN.TextureCoords).rgb * CONSTANT.Albedo.rgb * IN.Color.rgb;
				float roughness = max(CONSTANT.Roughness, 0.05);
				vec3 normal = normalize(IN.Normal);

				if (CONSTANT.UseNormalMap)
				{
					normal = normalize(texture(_NormalMap, IN.TextureCoords).rgb * 2.0 - 1.0);
					normal = normalize(IN.WorldNormal * normal);
				}

				vec3 view = normalize(u_Camera.Position.xyz - IN.WorldPosition);
				float NdotV = max(dot(normal, view), 0.0);

				vec3 Lr = 2.0 * NdotV * normal - view;
				vec3 F0 = mix(dielectric, albedo, CONSTANT.Metalness);

				//Light calculations
				vec3 Lo = vec3(0.0);
				
				//Calculate final color
				vec3 ibl = IBL(F0, Lr, IN.Normal, albedo, NdotV, roughness, CONSTANT.Metalness);

				vec3 color = IN.Color.rgb;
				color = ACESTonemap(ibl + Lo);
				color = GammaCorrect(color, gamma);

				OUT.Color = vec4(color, 1.0);
				OUT.EntityID = IN.EntityID;
			}
		}
	}
}
