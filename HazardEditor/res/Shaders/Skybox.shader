Shader "Skybox/Default"
{
	Language "GLSL"
	Type "Fullscreen"
	Version 450

	Depth "Less"
	DepthWrite True

	Properties 
	{
		u_Camera ("Camera", Buffer, 0, 0, 1)
		u_RadianceMap ("RadianceMap", SamplerCube, 0, 1, 1)
		u_IrradianceMap ("IrradianceMap", SamplerCube, 0, 2, 1)
		u_BRDFLut ("BRDFLut", Sampler2D, 0, 3, 1)
	}

	Constants 
	{
		LodLevel ("LodLevel", float)
		Intensity ("Intensity", float)
	}

	Vertex
	{
		Include 
		{
			"Uniforms/CameraUniform.glslh"
		}

		Shader 
		{			
			struct VS_Out : VS_OUT 
			{
				vec3 Position;
			};

			vec3 quadPos[6] = vec3[](
				vec3( 1,  1, 0), vec3(-1, -1, 0), vec3(-1,  1, 0),
				vec3(-1, -1, 0), vec3( 1,  1, 0), vec3( 1, -1, 0)
			);

			void main() 
			{
				vec4 position = vec4(quadPos[gl_VertexIndex].xy, 1.0, 1.0);
				gl_Position = position;

				vec4 inversedPos = position;
				OUT.Position = (mat4(mat3(inverse(u_Camera.View))) * inverse(u_Camera.Projection) * inversedPos).xyz;
			}
		}
	}

	Fragment
	{

		Include
		{
			"Utils/PostProcessing.glslh"
		}

		Shader
		{
			struct VS_Out : FS_IN
			{
				vec3 Position;
			};

			struct FS_Out : FS_OUT
			{
				vec4 Color;
				uint EntityID;
			};

			const float gamma = 2.2;

			void main() 
			{
				int levelTexels = textureQueryLevels(u_RadianceMap);
				vec3 color = textureLod(u_RadianceMap, IN.Position, (CONSTANT.LodLevel * levelTexels)).rgb;
				color *= CONSTANT.Intensity;
				color = GammaCorrect(color, gamma);

				OUT.Color = vec4(ACESTonemap(color), 1.0);

				OUT.EntityID = 0;
			}
		}
	}
}