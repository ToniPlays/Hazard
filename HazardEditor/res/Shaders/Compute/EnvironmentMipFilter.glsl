#type Compute
#version 450 core

#include "../Utils/Common.glsl"
#include "../Utils/EnvironmentMapping.glsl"

const uint Samples = 1024;
const int MipLevels = 1;

layout(binding = 0, rgba16f) restrict writeonly uniform imageCube o_Texture[MipLevels];
layout(binding = 1) uniform samplerCube u_InputCube;

#define PARAM_LEVEL 0
#define PARAM_ROUGHNESS 0.5

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main(void)
{
	ivec2 outputSize = imageSize(o_Texture[PARAM_LEVEL]);
	if(gl_GlobalInvocationID.x >= outputSize.x || gl_GlobalInvocationID.y >= outputSize.y) return;

	//Solid angle associated with a single cubemap texel at mip 0
	vec2 inputSize = vec2(textureSize(u_InputCube, 0));
	float WT = 4.0 * PI / (6 * inputSize.x * inputSize.y);

	vec3 N = GetCubeMapTextureCoord(imageSize(o_Texture[PARAM_LEVEL]).xy);
	vec3 Lo = N;

	vec3 S, T;
	ComputeBasisVectors(N, S, T);

	vec3 color = vec3(0.0);
	float weight = 0.0;

	//Convolve environment map using GGX NDF importance sampling
	for(uint i = 0; i < Samples; i++)
	{
		vec2 u = SampleHammersley(i, Samples);
		vec3 Lh = TangentToWorld(SampleGGX(u.x, u.y, PARAM_ROUGHNESS), N, S, T);

		vec3 Li = 2.0 * dot(Lo, Lh) * Lh - Lo;

		float cosLi = dot(N, Li);
		if(cosLi > 0.0)
		{
			float cosLh = max(dot(N, Lh), 0.0);

			float pdf = NdfGGX(cosLh, PARAM_ROUGHNESS) * 0.25;
			
			float WS = 1.0 / (Samples * pdf);

			float mipLevel = max(0.5 * log2(WS / WT) + 1.0, 0.0);

			color += textureLod(u_InputCube, Li, mipLevel).rgb * cosLi;
			weight += cosLi;
		}
	}
	color /= weight;
	imageStore(o_Texture[PARAM_LEVEL], ivec3(gl_GlobalInvocationID), vec4(color, 1.0));
}
