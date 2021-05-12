#type Vertex
#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec2 v_tex_coords;
layout(location = 3) in float v_tex_index;

uniform mat4 viewProjection;

out vec4 f_color;
out vec2 texCoords;
flat out float textureIndex;

void main() {

	f_color = v_color;
	texCoords = v_tex_coords;
	textureIndex = v_tex_index;

	gl_Position = viewProjection * vec4(v_position, 1.0);
}

#type Fragment
#version 330 core

in vec4 f_color;
in vec2 texCoords;
flat in float textureIndex;

uniform sampler2D u_Textures[8];

out vec4 color;

void main() {

	vec4 c = f_color;

	int index = int(textureIndex);

	switch(index)
	{
		case 0: c *= texture(u_Textures[0], texCoords); break;
		case 1: c *= texture(u_Textures[1], texCoords); break;
		case 2: c *= texture(u_Textures[2], texCoords); break;
		case 3: c *= texture(u_Textures[3], texCoords); break;
		case 4: c *= texture(u_Textures[4], texCoords); break;
		case 5: c *= texture(u_Textures[5], texCoords); break;
		case 6: c *= texture(u_Textures[6], texCoords); break;
		case 7: c *= texture(u_Textures[7], texCoords); break;
	}

	color = c;
}