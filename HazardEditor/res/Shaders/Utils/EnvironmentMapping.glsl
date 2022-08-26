

vec3 GetCubeMapTextureCoord(vec2 imageSize) 
{
	vec2 st = gl_GlobalInvocationID.xy / imageSize;
	vec2 uv = 2.0 * vec2(st.x, 1.0 - st.y) - vec2(1.0);

	switch(gl_GlobalInvocationID.z) 
	{
		case 0: return vec3( 1.0, uv.y, -uv.x);
		case 1: return vec3(-1.0, uv.y,  uv.x);
		case 2: return vec3( uv.x,  1.0,-uv.y);
		case 3: return vec3( uv.x, -1.0, uv.y);
		case 4: return vec3( uv.x, uv.y, 1.0);
		case 5: return vec3(-uv.x, uv.y,-1.0);
	}
	return vec3(1.0);
}

float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}


void ComputeBasisVectors(const vec3 N, out vec3 S, out vec3 T) 	
{
	T = cross(N, vec3(0.0, 1.0, 0.0));
	T = mix(cross(N, vec3(1.0, 0.0, 0.0)), T, step(Epsilon, dot(T, T)));

	T = normalize(T);
	S = normalize(cross(N, T));
}

vec2 SampleHammersley(uint i, uint samples) 
{
	float invSamples = 1.0 / float(samples);
	return vec2(i * invSamples, RadicalInverse_VdC(i));
}
vec3 TangentToWorld(const vec3 v, const vec3 N, const vec3 S, const vec3 T) 
{
	return S * v.x + T * v.y + N * v.z;
}
vec3 SampleHemisphere(float u1, float u2) 
{
	const float u1p = sqrt(max(0.0, 1.0 - u1 * u1));
	return vec3(cos(TwoPI * u2) * u1p, sin(TwoPI * u2) * u1p, u1);
}