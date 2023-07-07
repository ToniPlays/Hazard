#type Compute

#version 450 core

#include "../Utils/Common.glslh"
#include "../Utils/EnvironmentMapping.glslh"

layout(set = 0, binding = 0, rgba16f) restrict writeonly uniform imageCube o_CubeMap;
layout(set = 0, binding = 1) uniform sampler2D u_EquirectangularTexture;

layout(local_size_x = 32, local_size_y = 32, local_size_y = 1) in;
void main() 
{
	vec3 cubeTexCoord = GetCubeMapTextureCoord(vec2(imageSize(o_CubeMap)));
	cubeTexCoord = normalize(cubeTexCoord);

	float phi = atan(cubeTexCoord.z, cubeTexCoord.x);
	float theta = acos(cubeTexCoord.y);
	vec2 uv = vec2(phi / (2.0 * PI) + 0.5, theta / PI);
	
	//Flip source image
	uv.y = 1.0 - uv.y;

	vec4 color = textureLod(u_EquirectangularTexture, uv, 0.0);
	imageStore(o_CubeMap, ivec3(gl_GlobalInvocationID), color);
}
