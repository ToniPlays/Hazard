Shader "2D/QuadShader"
{
	Language "GLSL"
	Type "2D/Quad"
	Version 450

	Depth "Less"
	DepthWrite True

	Properties
	{
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
				gl_Position = u_Camera.ViewProjection * vec4(IN.Position, 1.0);
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
	
				vec4 textureColor = vec4(1.0);
	
				switch(index) {
					case  0: 	 textureColor = texture(u_Textures[ 0], IN.TextureCoords);  break;
					case  1: 	 textureColor = texture(u_Textures[ 1], IN.TextureCoords);  break;
					case  2: 	 textureColor = texture(u_Textures[ 2], IN.TextureCoords);  break;
					case  3: 	 textureColor = texture(u_Textures[ 3], IN.TextureCoords);  break;
					case  4: 	 textureColor = texture(u_Textures[ 4], IN.TextureCoords);  break;
					case  5: 	 textureColor = texture(u_Textures[ 5], IN.TextureCoords);  break;
					case  6: 	 textureColor = texture(u_Textures[ 6], IN.TextureCoords);  break;
					case  7: 	 textureColor = texture(u_Textures[ 7], IN.TextureCoords);  break;
					case  8: 	 textureColor = texture(u_Textures[ 8], IN.TextureCoords);  break;
					case  9: 	 textureColor = texture(u_Textures[ 9], IN.TextureCoords);  break;
					case 10: 	 textureColor = texture(u_Textures[10], IN.TextureCoords);  break;
					case 11: 	 textureColor = texture(u_Textures[11], IN.TextureCoords);  break;
					case 12: 	 textureColor = texture(u_Textures[12], IN.TextureCoords);  break;
					case 13: 	 textureColor = texture(u_Textures[13], IN.TextureCoords);  break;
					case 14: 	 textureColor = texture(u_Textures[14], IN.TextureCoords);  break;
					case 15: 	 textureColor = texture(u_Textures[15], IN.TextureCoords);  break;
					case 16: 	 textureColor = texture(u_Textures[16], IN.TextureCoords);  break;
					case 17: 	 textureColor = texture(u_Textures[17], IN.TextureCoords);  break;
					case 18: 	 textureColor = texture(u_Textures[18], IN.TextureCoords);  break;
					case 19: 	 textureColor = texture(u_Textures[19], IN.TextureCoords);  break;
					case 20: 	 textureColor = texture(u_Textures[20], IN.TextureCoords);  break;
					case 21: 	 textureColor = texture(u_Textures[21], IN.TextureCoords);  break;
					case 22: 	 textureColor = texture(u_Textures[22], IN.TextureCoords);  break;
					case 23: 	 textureColor = texture(u_Textures[23], IN.TextureCoords);  break;
					case 24: 	 textureColor = texture(u_Textures[24], IN.TextureCoords);  break;
					case 25: 	 textureColor = texture(u_Textures[25], IN.TextureCoords);  break;
					case 26: 	 textureColor = texture(u_Textures[26], IN.TextureCoords);  break;
					case 27: 	 textureColor = texture(u_Textures[27], IN.TextureCoords);  break;
					case 28: 	 textureColor = texture(u_Textures[28], IN.TextureCoords);  break;
					case 29: 	 textureColor = texture(u_Textures[29], IN.TextureCoords);  break;
					case 30: 	 textureColor = texture(u_Textures[30], IN.TextureCoords);  break;
					case 31: 	 textureColor = texture(u_Textures[31], IN.TextureCoords);  break;
				}

				OUT.Color = textureColor * IN.Color;

				if(OUT.Color.a <= 0.0001f) discard;
			}
		}
	}
}