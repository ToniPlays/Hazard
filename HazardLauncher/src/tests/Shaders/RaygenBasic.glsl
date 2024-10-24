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

void main()
{
	//Interpolate
	const vec3 barycentricCoords = vec3(1.0 - hitAttribs.x - hitAttribs.y, hitAttribs.x, hitAttribs.y);

	hitValue = barycentricCoords;
}