Shader "2D/QuadShader"
{
	Language "GLSL"
	Type "2D/Quad"
	Version 450

	Depth "Less"
	DepthWrite True
	
	Properties
	{
		u_Camera ("Camera", Buffer, 0, 0, 1)
		u_RadianceMap ("RadianceMap", SamplerCube, 0, 1, 1)
		u_IrradianceMap ("IrradianceMap", SamplerCube, 0, 2, 1)
		u_BRDFLut ("BRDFLut", Sampler2D, 0, 3, 1)
		u_Textures ("Textures", Sampler2D, 1, 0, 32)
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
				vec2 TextureCoords;
				float TextureIndex;
			};

			struct VS_Out : VS_OUT 
			{
				vec4 Color;
				vec2 TextureCoords;
				flat float TextureIndex;
			};

			void main() 
			{
				OUT.Color = IN.Color;
				OUT.TextureCoords = IN.TextureCoords;
				OUT.TextureIndex = IN.TextureIndex;
				vec4 screenPos = u_Camera.ViewProjection * vec4(IN.Position, 1.0);
				gl_Position = screenPos;
			}
		}
	}

	Fragment
	{
		Shader
		{
			struct FS_In : FS_IN
			{
				vec4 Color;
				vec2 TextureCoords;
				flat float TextureIndex;
			};

			struct FS_Out : FS_OUT
			{
				vec4 Color;
				uint EntityID;
			};

			void main() 
			{
				int index = int(IN.TextureIndex);
				vec2 uv = IN.TextureCoords;
				vec4 textureColor = vec4(1.0);
	
				switch(index) {
					case  0: 	 textureColor = texture(u_Textures[ 0], uv);  break;
					case  1: 	 textureColor = texture(u_Textures[ 1], uv);  break;
					case  2: 	 textureColor = texture(u_Textures[ 2], uv);  break;
					case  3: 	 textureColor = texture(u_Textures[ 3], uv);  break;
					case  4: 	 textureColor = texture(u_Textures[ 4], uv);  break;
					case  5: 	 textureColor = texture(u_Textures[ 5], uv);  break;
					case  6: 	 textureColor = texture(u_Textures[ 6], uv);  break;
					case  7: 	 textureColor = texture(u_Textures[ 7], uv);  break;
					case  8: 	 textureColor = texture(u_Textures[ 8], uv);  break;
					case  9: 	 textureColor = texture(u_Textures[ 9], uv);  break;
					case 10: 	 textureColor = texture(u_Textures[10], uv);  break;
					case 11: 	 textureColor = texture(u_Textures[11], uv);  break;
					case 12: 	 textureColor = texture(u_Textures[12], uv);  break;
					case 13: 	 textureColor = texture(u_Textures[13], uv);  break;
					case 14: 	 textureColor = texture(u_Textures[14], uv);  break;
					case 15: 	 textureColor = texture(u_Textures[15], uv);  break;
					case 16: 	 textureColor = texture(u_Textures[16], uv);  break;
					case 17: 	 textureColor = texture(u_Textures[17], uv);  break;
					case 18: 	 textureColor = texture(u_Textures[18], uv);  break;
					case 19: 	 textureColor = texture(u_Textures[19], uv);  break;
					case 20: 	 textureColor = texture(u_Textures[20], uv);  break;
					case 21: 	 textureColor = texture(u_Textures[21], uv);  break;
					case 22: 	 textureColor = texture(u_Textures[22], uv);  break;
					case 23: 	 textureColor = texture(u_Textures[23], uv);  break;
					case 24: 	 textureColor = texture(u_Textures[24], uv);  break;
					case 25: 	 textureColor = texture(u_Textures[25], uv);  break;
					case 26: 	 textureColor = texture(u_Textures[26], uv);  break;
					case 27: 	 textureColor = texture(u_Textures[27], uv);  break;
					case 28: 	 textureColor = texture(u_Textures[28], uv);  break;
					case 29: 	 textureColor = texture(u_Textures[29], uv);  break;
					case 30: 	 textureColor = texture(u_Textures[30], uv);  break;
					case 31: 	 textureColor = texture(u_Textures[31], uv);  break;
				}

				OUT.Color = textureColor * IN.Color;

				if(OUT.Color.a <= 0.0001f) discard;
			}
		}
	}
}