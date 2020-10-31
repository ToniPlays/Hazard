#type Vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 v_coords;
layout(location = 2) in vec4 color;
layout(location = 3) in float textureIndex;

uniform mat4 viewProjection;

out vec4 v_color;
out vec2 o_coords;
out float texIndex;

void main() {
	
	gl_Position = viewProjection * vec4(position, 1.0);
	v_color = color;
	o_coords = v_coords;
	texIndex = textureIndex;
}

#type Fragment
#version 330 core


in vec4 v_color;
in vec2 o_coords;
in float texIndex;

uniform sampler2D[16] textures;

out vec4 out_color;

void main() {
	
	vec4 texColor = v_color;
	switch(int(texIndex))
	{
		case 0:  texColor *= texture(textures[0], o_coords); break;
		case 1:  texColor *= texture(textures[1], o_coords); break;
		case 2:  texColor *= texture(textures[2], o_coords); break;
		case 3:  texColor *= texture(textures[3], o_coords); break;
		case 4:  texColor *= texture(textures[4], o_coords); break;
		case 5:  texColor *= texture(textures[5], o_coords); break;
		case 6:  texColor *= texture(textures[6], o_coords); break;
		case 7:  texColor *= texture(textures[7], o_coords); break;
		case 8:  texColor *= texture(textures[8], o_coords); break;
		case 9:  texColor *= texture(textures[9], o_coords); break;
		case 10: texColor *= texture(textures[10], o_coords); break;
		case 11: texColor *= texture(textures[11], o_coords); break;
		case 12: texColor *= texture(textures[12], o_coords); break;
		case 13: texColor *= texture(textures[13], o_coords); break;
		case 14: texColor *= texture(textures[14], o_coords); break;
		case 15: texColor *= texture(textures[15], o_coords); break;
	}
	out_color = v_color * texColor;
}