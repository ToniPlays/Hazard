Shader "Debug/LineShader"
{
	Language "GLSL"
	Type "Line"
	Version 450

	Pass {
		Depth "<"
		DepthWrite True
		
		CullFace "None"
		Queue "Opaque"
	}

	Properties 
	{
		u_Camera ("Camera", CameraBlock, 0, 0, 1)
		u_RadianceMap ("RadianceMap", samplerCube, 0, 1, 1)
		u_IrradianceMap ("IrradianceMap", samplerCube, 0, 2, 1)
		u_BRDFLut ("BRDFLut", sampler2D, 0, 3, 1)
	}

	Vertex
	{
		Include 
		{
			"../Uniforms/CameraUniform.glslh"
		}

		Source
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
		Include 
		{
			"../Uniforms/CameraUniform.glslh"
		}

		Source
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
