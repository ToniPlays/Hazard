#version 450 core

in vec4 f_color;
in vec2 texCoords;
flat in float textureIndex;

uniform sampler2D u_Textures[32];

out vec4 color;

void main() {

	vec4 c = f_color;

	int index = int(textureIndex);

	switch(index)
	{
		case 0:  c *= texture(u_Textures[ 0], texCoords); break;
		case 1:  c *= texture(u_Textures[ 1], texCoords); break;
		case 2:  c *= texture(u_Textures[ 2], texCoords); break;
		case 3:  c *= texture(u_Textures[ 3], texCoords); break;
		case 4:  c *= texture(u_Textures[ 4], texCoords); break;
		case 5:  c *= texture(u_Textures[ 5], texCoords); break;
		case 6:  c *= texture(u_Textures[ 6], texCoords); break;
		case 7:  c *= texture(u_Textures[ 7], texCoords); break;
		case 8:  c *= texture(u_Textures[ 8], texCoords); break;
		case 9:  c *= texture(u_Textures[ 9], texCoords); break;
		case 10: c *= texture(u_Textures[10], texCoords); break;
		case 11: c *= texture(u_Textures[11], texCoords); break;
		case 12: c *= texture(u_Textures[12], texCoords); break;
		case 13: c *= texture(u_Textures[13], texCoords); break;
		case 14: c *= texture(u_Textures[14], texCoords); break;
		case 15: c *= texture(u_Textures[15], texCoords); break;
		case 16: c *= texture(u_Textures[16], texCoords); break;
		case 17: c *= texture(u_Textures[17], texCoords); break;
		case 18: c *= texture(u_Textures[18], texCoords); break;
		case 19: c *= texture(u_Textures[19], texCoords); break;
		case 20: c *= texture(u_Textures[20], texCoords); break;
		case 21: c *= texture(u_Textures[21], texCoords); break;
		case 22: c *= texture(u_Textures[22], texCoords); break;
		case 23: c *= texture(u_Textures[23], texCoords); break;
		case 24: c *= texture(u_Textures[24], texCoords); break;
		case 25: c *= texture(u_Textures[25], texCoords); break;
		case 26: c *= texture(u_Textures[26], texCoords); break;
		case 27: c *= texture(u_Textures[27], texCoords); break;
		case 28: c *= texture(u_Textures[28], texCoords); break;
		case 29: c *= texture(u_Textures[29], texCoords); break;
		case 30: c *= texture(u_Textures[30], texCoords); break;
		case 31: c *= texture(u_Textures[31], texCoords); break;
	}
	color = c;
}