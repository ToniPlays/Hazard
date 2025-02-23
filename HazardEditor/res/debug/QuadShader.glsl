#type Fragment
#version 450

layout (set = 1, binding = 0) uniform sampler2D u_Textures[32];


			layout(location = 0) in vec4 IN_Color;
			layout(location = 1) in vec2 IN_TextureCoords;
			layout(location = 2) in flat float IN_TextureIndex;
			

			layout(location = 0) out vec4 OUT_Color;
			layout(location = 1) out uint OUT_EntityID;
			

			void main() 
			{
				int index = int(IN_TextureIndex);
				vec2 uv = IN_TextureCoords;
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

				OUT_Color = textureColor * IN_Color;

				if(OUT_Color.a <= 0.0001f) discard;
			}

#type Vertex
#version 450
#include "../Uniforms/CameraUniform.glslh"



			layout(location = 0) in vec3 IN_Position;
			layout(location = 1) in vec4 IN_Color;
			layout(location = 2) in vec2 IN_TextureCoords;
			layout(location = 3) in float IN_TextureIndex;
			

			layout(location = 0) out vec4 OUT_Color;
			layout(location = 1) out vec2 OUT_TextureCoords;
			layout(location = 2) out flat float OUT_TextureIndex;
			

			void main() 
			{
				OUT_Color = IN_Color;
				OUT_TextureCoords = IN_TextureCoords;
				OUT_TextureIndex = IN_TextureIndex;
				vec4 screenPos = u_Camera.ViewProjection * vec4(IN_Position, 1.0);
				gl_Position = screenPos;
			}

