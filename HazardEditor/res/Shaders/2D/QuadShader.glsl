#type Vertex
#version 450

#include "../Uniforms/CameraUniform.glslh"
#include "../Inputs/InputQuad.glslh"

layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TextureCoords;
layout(location = 2) out float v_TextureIndex;

layout(push_constant) uniform Transform
{
    mat4 Transform;
} p_Transform;

void main() 
{
	v_Color = a_Color;
	v_TextureCoords = a_TextureCoords;
	v_TextureIndex = a_TextureIndex;
	gl_Position = u_Camera.ViewProjection * vec4(a_Position, 1.0);
}

#type Fragment
#version 450

layout(location = 0) in vec4 v_Color;
layout(location = 1) in vec2 v_TextureCoords;
layout(location = 2) in flat float v_TextureIndex;

layout(set = 0, binding = 1) uniform samplerCube u_RadianceMap;
layout(set = 0, binding = 2) uniform samplerCube u_IrradianceMap;
layout(set = 0, binding = 3) uniform sampler2D u_BRDFLut;

layout (set = 1, binding = 0) uniform sampler2D u_Textures[32];

layout(location = 0) out vec4 Color;
layout(location = 1) out uint EntityID;

void main() 
{
	int index = int(v_TextureIndex);
	
	vec4 textureColor = vec4(1.0);
	
	switch(index) {
		case  0: 	 textureColor = texture(u_Textures[ 0], v_TextureCoords);  break;
		case  1: 	 textureColor = texture(u_Textures[ 1], v_TextureCoords);  break;
		case  2: 	 textureColor = texture(u_Textures[ 2], v_TextureCoords);  break;
		case  3: 	 textureColor = texture(u_Textures[ 3], v_TextureCoords);  break;
		case  4: 	 textureColor = texture(u_Textures[ 4], v_TextureCoords);  break;
		case  5: 	 textureColor = texture(u_Textures[ 5], v_TextureCoords);  break;
		case  6: 	 textureColor = texture(u_Textures[ 6], v_TextureCoords);  break;
		case  7: 	 textureColor = texture(u_Textures[ 7], v_TextureCoords);  break;
		case  8: 	 textureColor = texture(u_Textures[ 8], v_TextureCoords);  break;
		case  9: 	 textureColor = texture(u_Textures[ 9], v_TextureCoords);  break;
		case 10: 	 textureColor = texture(u_Textures[10], v_TextureCoords);  break;
		case 11: 	 textureColor = texture(u_Textures[11], v_TextureCoords);  break;
		case 12: 	 textureColor = texture(u_Textures[12], v_TextureCoords);  break;
		case 13: 	 textureColor = texture(u_Textures[13], v_TextureCoords);  break;
		case 14: 	 textureColor = texture(u_Textures[14], v_TextureCoords);  break;
		case 15: 	 textureColor = texture(u_Textures[15], v_TextureCoords);  break;
		case 16: 	 textureColor = texture(u_Textures[16], v_TextureCoords);  break;
		case 17: 	 textureColor = texture(u_Textures[17], v_TextureCoords);  break;
		case 18: 	 textureColor = texture(u_Textures[18], v_TextureCoords);  break;
		case 19: 	 textureColor = texture(u_Textures[19], v_TextureCoords);  break;
		case 20: 	 textureColor = texture(u_Textures[20], v_TextureCoords);  break;
		case 21: 	 textureColor = texture(u_Textures[21], v_TextureCoords);  break;
		case 22: 	 textureColor = texture(u_Textures[22], v_TextureCoords);  break;
		case 23: 	 textureColor = texture(u_Textures[23], v_TextureCoords);  break;
		case 24: 	 textureColor = texture(u_Textures[24], v_TextureCoords);  break;
		case 25: 	 textureColor = texture(u_Textures[25], v_TextureCoords);  break;
		case 26: 	 textureColor = texture(u_Textures[26], v_TextureCoords);  break;
		case 27: 	 textureColor = texture(u_Textures[27], v_TextureCoords);  break;
		case 28: 	 textureColor = texture(u_Textures[28], v_TextureCoords);  break;
		case 29: 	 textureColor = texture(u_Textures[29], v_TextureCoords);  break;
		case 30: 	 textureColor = texture(u_Textures[30], v_TextureCoords);  break;
		case 31: 	 textureColor = texture(u_Textures[31], v_TextureCoords);  break;
	}
	Color = textureColor * v_Color;

	if(Color.a <= 0.0001f) discard;

	EntityID = 0;
}