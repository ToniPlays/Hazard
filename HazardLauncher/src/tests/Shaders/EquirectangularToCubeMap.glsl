#type Compute

#version 450 core

const float PI = 3.14159265359;
const float TwoPI = 2 * PI;
const float Epsilon = 0.00001;

vec3 RotateVectorAboutY(float angle, vec3 vec)
{
	angle = radians(angle);
	mat3x3 rotationMatrix = { vec3(cos(angle),0.0,sin(angle)),
							vec3(0.0,1.0,0.0),
							vec3(-sin(angle),0.0,cos(angle)) };
	return rotationMatrix * vec;
}



vec3 GetCubeMapTextureCoord(vec2 imageSize) 
{
	vec2 st = gl_GlobalInvocationID.xy / imageSize;
	vec2 uv = 2.0 * vec2(st.x, 1.0 - st.y) - vec2(1.0);

	vec3 ret;
    if (gl_GlobalInvocationID.z == 0)      ret = vec3(  1.0, uv.y, -uv.x);
    else if (gl_GlobalInvocationID.z == 1) ret = vec3( -1.0, uv.y,  uv.x);
    else if (gl_GlobalInvocationID.z == 2) ret = vec3( uv.x,  1.0, -uv.y);
    else if (gl_GlobalInvocationID.z == 3) ret = vec3( uv.x, -1.0,  uv.y);
    else if (gl_GlobalInvocationID.z == 4) ret = vec3( uv.x, uv.y,   1.0);
    else if (gl_GlobalInvocationID.z == 5) ret = vec3(-uv.x, uv.y,  -1.0);
    return normalize(ret);
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

vec3 SampleGGX(float u1, float u2, float roughness)
{
	float alpha = roughness * roughness;

	float cosTheta = sqrt((1.0 - u2) / (1.0 + (alpha * alpha - 1.0) * u2));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	float phi = TwoPI * u1;

	//Convert to Cartesian upon return
	return vec3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
}
float NdfGGX(float cosLh, float roughness)
{
	float alpha = roughness * roughness;
	float alpha2 = alpha * alpha;

	float denom = (cosLh * cosLh) * (alpha2 - 1.0) + 1.0;
	return alpha2 / (PI * denom * denom);
}

layout(binding = 0, rgba16f) restrict writeonly uniform imageCube o_CubeMap;
layout(binding = 1) uniform sampler2D u_EquirectangularTexture;

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
