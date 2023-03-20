#type Vertex
#version 450

#include "../Uniforms/CameraUniform.glslh"
#include "../Inputs/InputQuad.glslh"

layout(location = 0) out vec4 Color;
layout(location = 1) out vec2 TextureCoords;
layout(location = 2) out float TextureIndex;

void main() 
{
	Color = a_Color;
	TextureCoords = a_TextureCoords;
	TextureIndex = a_TextureIndex;
	gl_Position = u_Camera.ViewProjection * vec4(a_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 Color;
layout(location = 1) in vec2 TextureCoords;
layout(location = 2) in flat float TextureIndex;

layout (set = 1, binding = 1) uniform sampler2D u_Textures[32];

layout(location = 0) out vec4 color;

void main() 
{
	int index = int(TextureIndex);
	
	vec4 textureColor = vec4(1.0);
	
	switch(index) {
		case  0: 	 textureColor = texture(u_Textures[ 0], TextureCoords);  break;
		case  1: 	 textureColor = texture(u_Textures[ 1], TextureCoords);  break;
		case  2: 	 textureColor = texture(u_Textures[ 2], TextureCoords);  break;
		case  3: 	 textureColor = texture(u_Textures[ 3], TextureCoords);  break;
		case  4: 	 textureColor = texture(u_Textures[ 4], TextureCoords);  break;
		case  5: 	 textureColor = texture(u_Textures[ 5], TextureCoords);  break;
		case  6: 	 textureColor = texture(u_Textures[ 6], TextureCoords);  break;
		case  7: 	 textureColor = texture(u_Textures[ 7], TextureCoords);  break;
		case  8: 	 textureColor = texture(u_Textures[ 8], TextureCoords);  break;
		case  9: 	 textureColor = texture(u_Textures[ 9], TextureCoords);  break;
		case 10: 	 textureColor = texture(u_Textures[10], TextureCoords);  break;
		case 11: 	 textureColor = texture(u_Textures[11], TextureCoords);  break;
		case 12: 	 textureColor = texture(u_Textures[12], TextureCoords);  break;
		case 13: 	 textureColor = texture(u_Textures[13], TextureCoords);  break;
		case 14: 	 textureColor = texture(u_Textures[14], TextureCoords);  break;
		case 15: 	 textureColor = texture(u_Textures[15], TextureCoords);  break;
		case 16: 	 textureColor = texture(u_Textures[16], TextureCoords);  break;
		case 17: 	 textureColor = texture(u_Textures[17], TextureCoords);  break;
		case 18: 	 textureColor = texture(u_Textures[18], TextureCoords);  break;
		case 19: 	 textureColor = texture(u_Textures[19], TextureCoords);  break;
		case 20: 	 textureColor = texture(u_Textures[20], TextureCoords);  break;
		case 21: 	 textureColor = texture(u_Textures[21], TextureCoords);  break;
		case 22: 	 textureColor = texture(u_Textures[22], TextureCoords);  break;
		case 23: 	 textureColor = texture(u_Textures[23], TextureCoords);  break;
		case 24: 	 textureColor = texture(u_Textures[24], TextureCoords);  break;
		case 25: 	 textureColor = texture(u_Textures[25], TextureCoords);  break;
		case 26: 	 textureColor = texture(u_Textures[26], TextureCoords);  break;
		case 27: 	 textureColor = texture(u_Textures[27], TextureCoords);  break;
		case 28: 	 textureColor = texture(u_Textures[28], TextureCoords);  break;
		case 29: 	 textureColor = texture(u_Textures[29], TextureCoords);  break;
		case 30: 	 textureColor = texture(u_Textures[30], TextureCoords);  break;
		case 31: 	 textureColor = texture(u_Textures[31], TextureCoords);  break;
	}
	color = textureColor * Color;
}
