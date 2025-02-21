Shader "3D/Lit_Static"
{
	Language "GLSL"
	Type "3D/Lit"
	Version 450

	Depth "LessOrEqual"
	DepthWrite True

	Properties 
	{
		u_Albedo ("Albedo", Sampler2D, 1, 0, 1)
		u_NormalMap ("NormalMap", Sampler2D, 1, 1, 1)
	}

	Vertex
	{
		Include 
		{
			"Uniforms/CameraUniform.glslh"
		}

		Shader 
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
				flat uint EntityID;
			};

			void main() 
			{
				mat4 transform = mat4(
					vec4(IN.MRow0.x, IN.MRow1.x, IN.MRow2.x, 0.0),
					vec4(IN.MRow0.y, IN.MRow1.y, IN.MRow2.y, 0.0),
					vec4(IN.MRow0.z, IN.MRow1.z, IN.MRow2.z, 0.0),
					vec4(IN.MRow0.w, IN.MRow1.w, IN.MRow2.w, 1.0)
				);

				vec4 worldPosition = transform * vec4(IN.Position, 1.0);
				gl_Position = u_Camera.ViewProjection * worldPosition;

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
		Include {
			"Uniforms/CameraUniform.glslh"
			"Utils/Common.glslh"
			"Utils/Lighting.glslh"
			"Utils/PostProcessing.glslh"
		}

		Shader
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

			/*layout(push_constant, std140) uniform PushConstants
			{
				uniform vec4 Albedo;
				uniform float Metalness;
				uniform float Roughness;
				uniform bool UseNormalMap;
				uniform bool Padding;
				uniform bool Padding1;
				uniform bool Padding2;
			} u_PushConstants;
			*/

			const float gamma = 2.2;
			const vec3 dielectric = vec3(0.04);

			void main() 
			{
				vec3 albedo = texture(u_Albedo, IN.TextureCoords).rgb * u_PushConstants.Albedo.rgb * IN.Color.rgb;
				float metalness = u_PushConstants.Metalness;
				float roughness = max(u_PushConstants.Roughness, 0.05);
				vec3 normal = normalize(IN.Normal);

				if (u_PushConstants.UseNormalMap)
				{
					normal = normalize(texture(u_NormalMap, IN.TextureCoords).rgb * 2.0 - 1.0);
					normal = normalize(IN.WorldNormal * normal);
				}

				vec3 view = normalize(u_Camera.Position.xyz - IN.WorldPosition);
				float NdotV = max(dot(normal, view), 0.0);

				vec3 Lr = 2.0 * NdotV * normal - view;
				vec3 F0 = mix(dielectric, albedo, metalness);

				//Light calculations
				vec3 Lo = vec3(0.0);
				
				//Calculate final color
				vec3 ibl = IBL(F0, Lr, IN.Normal, albedo, NdotV, roughness, metalness);

				vec3 color = IN.Color.rgb;
				color = ACESTonemap(ibl + Lo);
				color = GammaCorrect(color, gamma);

				OUT.Color = vec4(color, 1.0);
				OUT.EntityID = IN.EntityID;
			}
		}
	}
}
