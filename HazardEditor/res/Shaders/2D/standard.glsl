#type Vertex
#version 450

#include "../Uniforms/CameraUniform.glsl"
#include "../Inputs/InputQuad.glsl"

layout(location = 0) out vec4 f_Color;
layout(location = 1) out vec2 f_TextureCoords;
layout(location = 2) out float f_TextureIndex;

void main() 
{
	f_Color = a_Color;
	f_TextureCoords = a_TextureCoords;
	f_TextureIndex = a_TextureIndex;
	gl_Position = u_Camera.ViewProjection * vec4(a_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 f_Color;
layout(location = 1) in vec2 f_TextureCoords;
layout(location = 2) in flat float f_TextureIndex;

layout (binding = 0) uniform sampler2D u_Textures[32];

layout(location = 0) out vec4 color;

void main() 
{
	int index = int(f_TextureIndex);
	
	vec4 textureColor = vec4(1.0);
	
	switch(index) {
		case  0: 	 textureColor = texture(u_Textures[ 0], f_TextureCoords);  break;
		case  1: 	 textureColor = texture(u_Textures[ 1], f_TextureCoords);  break;
		case  2: 	 textureColor = texture(u_Textures[ 2], f_TextureCoords);  break;
		case  3: 	 textureColor = texture(u_Textures[ 3], f_TextureCoords);  break;
		case  4: 	 textureColor = texture(u_Textures[ 4], f_TextureCoords);  break;
		case  5: 	 textureColor = texture(u_Textures[ 5], f_TextureCoords);  break;
		case  6: 	 textureColor = texture(u_Textures[ 6], f_TextureCoords);  break;
		case  7: 	 textureColor = texture(u_Textures[ 7], f_TextureCoords);  break;
		case  8: 	 textureColor = texture(u_Textures[ 8], f_TextureCoords);  break;
		case  9: 	 textureColor = texture(u_Textures[ 9], f_TextureCoords);  break;
		case 10: 	 textureColor = texture(u_Textures[10], f_TextureCoords);  break;
		case 11: 	 textureColor = texture(u_Textures[11], f_TextureCoords);  break;
		case 12: 	 textureColor = texture(u_Textures[12], f_TextureCoords);  break;
		case 13: 	 textureColor = texture(u_Textures[13], f_TextureCoords);  break;
		case 14: 	 textureColor = texture(u_Textures[14], f_TextureCoords);  break;
		case 15: 	 textureColor = texture(u_Textures[15], f_TextureCoords);  break;
		case 16: 	 textureColor = texture(u_Textures[16], f_TextureCoords);  break;
		case 17: 	 textureColor = texture(u_Textures[17], f_TextureCoords);  break;
		case 18: 	 textureColor = texture(u_Textures[18], f_TextureCoords);  break;
		case 19: 	 textureColor = texture(u_Textures[19], f_TextureCoords);  break;
		case 20: 	 textureColor = texture(u_Textures[20], f_TextureCoords);  break;
		case 21: 	 textureColor = texture(u_Textures[21], f_TextureCoords);  break;
		case 22: 	 textureColor = texture(u_Textures[22], f_TextureCoords);  break;
		case 23: 	 textureColor = texture(u_Textures[23], f_TextureCoords);  break;
		case 24: 	 textureColor = texture(u_Textures[24], f_TextureCoords);  break;
		case 25: 	 textureColor = texture(u_Textures[25], f_TextureCoords);  break;
		case 26: 	 textureColor = texture(u_Textures[26], f_TextureCoords);  break;
		case 27: 	 textureColor = texture(u_Textures[27], f_TextureCoords);  break;
		case 28: 	 textureColor = texture(u_Textures[28], f_TextureCoords);  break;
		case 29: 	 textureColor = texture(u_Textures[29], f_TextureCoords);  break;
		case 30: 	 textureColor = texture(u_Textures[30], f_TextureCoords);  break;
		case 31: 	 textureColor = texture(u_Textures[31], f_TextureCoords);  break;
	}
	
	color = textureColor * f_Color;
}