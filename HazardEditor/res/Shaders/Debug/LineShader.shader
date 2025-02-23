Shader "Debug/LineShader"
{
	Language "GLSL"
	Type "Line"
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

	Vertex
	{
		Include 
		{
			"../Uniforms/CameraUniform.glslh"
		}

		Shader 
		{			
			struct VS_In : VS_IN
			{
				vec3 Position;
				vec4 Color;
			};

			struct VS_Out : VS_OUT 
			{
				vec4 Color;
			};

			void main() 
			{
				OUT.Color = IN.Color;
				gl_Position = u_Camera.ViewProjection * vec4(IN.Position, 1.0);
			}
		}
	}

	Fragment
	{
		Shader
		{
			struct VS_Out : FS_IN
			{
				vec4 Color;
			};

			struct FS_Out : FS_OUT
			{
				vec4 Color;
				uint EntityID;
			};

			void main() 
			{
				OUT.Color = IN.Color;
				OUT.EntityID = 0;
			}
		}
	}
}