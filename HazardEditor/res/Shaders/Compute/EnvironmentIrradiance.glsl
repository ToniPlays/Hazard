#type Compute
#version 450 core

#include "../Utils/Common.glslh"
#include "../Utils/EnvironmentMapping.glslh"

layout(binding = 0, rgba16f) restrict writeonly uniform imageCube o_IrradianceMap;
layout(binding = 1) uniform samplerCube u_RadianceMap;

const uint u_Samples = 1024;

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main() 
{
	vec3 N = GetCubeMapTextureCoord(vec2(imageSize(o_IrradianceMap)));

	vec3 S, T;
	ComputeBasisVectors(N, S, T);

	uint samples = 64 * u_Samples;

	vec3 irradiance = vec3(0.0);
	for(uint i = 0; i < samples; i++) 
	{
		vec2 u = SampleHammersley(i, samples);
		vec3 Li = TangentToWorld(SampleHemisphere(u.x, u.y), N, S, T);
		float cosTheta = max(0.0, dot(Li, N));

		irradiance += 2.0 * textureLod(u_RadianceMap, Li, 0).rgb * cosTheta;
	}

	irradiance /= vec3(samples);
	imageStore(o_IrradianceMap, ivec3(gl_GlobalInvocationID), vec4(irradiance, 1.0));
}