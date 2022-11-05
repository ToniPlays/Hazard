#type Raygen

#version 460
#extension GL_EXT_ray_tracing : enable

layout(set = 0, binding = 0) uniform accelerationStructureEXT topLevelAS;
layout(set = 0, binding = 1, rgba8) uniform writeonly image2D image;
layout(set = 0, binding = 2) uniform Camera
{
	uniform mat4 ViewInverse;
	uniform mat4 ProjInverse;
} u_Camera;

layout(location = 0) rayPayloadEXT vec3 hitValue;

void main() 
{
	const vec2 pixelCenter = vec2(gl_LaunchIDEXT.xy) + vec2(0.5);
	const vec2 inUV = pixelCenter/vec2(gl_LaunchSizeEXT.xy);
	vec2 d = inUV * 2.0 - 1.0;

	vec4 origin = u_Camera.ViewInverse * vec4(0,0,0,1);
	vec4 target = u_Camera.ProjInverse * vec4(d.x, d.y, 1, 1) ;
	vec4 direction = u_Camera.ViewInverse * vec4(normalize(target.xyz), 0) ;

	float tmin = 0.001;
	float tmax = 10000.0;

    hitValue = vec3(0.0);

    traceRayEXT(topLevelAS, gl_RayFlagsOpaqueEXT, 0xff, 0, 0, 0, origin.xyz, tmin, direction.xyz, tmax, 0);

	imageStore(image, ivec2(gl_LaunchIDEXT.xy), vec4(hitValue, 0.0));
}

#type Miss

#version 460
#extension GL_EXT_ray_tracing : enable

layout(location = 0) rayPayloadInEXT vec3 hitValue;

void main() 
{
	hitValue = vec3(0.0, 0.0, 0.1);
}

#type ClosestHit

#version 460
#extension GL_EXT_ray_tracing : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) rayPayloadInEXT vec3 hitValue;

layout(set = 0, binding = 0) uniform accelerationStructureEXT topLevelAS;

hitAttributeEXT vec2 hitAttribs;

layout(set = 0, binding = 3) buffer Vertices { vec4 v[]; } vertices;
layout(set = 0, binding = 4) buffer Indices { uint i[]; } indices;

struct Vertex
{
	vec3 Position;
	vec4 Color;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TextureCoords;
	vec2 Padding;
};


Vertex Unpack(uint index)
{
	int m = 80 / 16;

	vec4 d0 = vertices.v[m * index + 0];
	vec4 d1 = vertices.v[m * index + 1];
	vec4 d2 = vertices.v[m * index + 2];

	Vertex v;
	v.Position = d0.xyz;
	v.Color = d2;
	v.Normal = vec3(d0.w, d1.x, d1.y);

	return v;
}
void main()
{
	ivec3 index = ivec3(indices.i[3 * gl_PrimitiveID], indices.i[3 * gl_PrimitiveID + 1], indices.i[3 * gl_PrimitiveID + 2]);
	
	Vertex v0 = Unpack(index.x);
	Vertex v1 = Unpack(index.y);
	Vertex v2 = Unpack(index.z);

	//Interpolate
	const vec3 barycentricCoords = vec3(1.0 - hitAttribs.x - hitAttribs.y, hitAttribs.x, hitAttribs.y);
	vec3 normal = normalize(v0.Normal * barycentricCoords.x + v1.Normal * barycentricCoords.y + v2.Normal * barycentricCoords.z);

	hitValue = vec3(normal);
}