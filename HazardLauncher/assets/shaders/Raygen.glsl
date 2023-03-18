#type Raygen

#version 460
#extension GL_EXT_ray_tracing : enable

layout(set = 0, binding = 0) uniform accelerationStructureEXT TLAS;
layout(set = 0, binding = 1, rgba8) uniform writeonly image2D outputImage;
layout(set = 0, binding = 2) uniform Camera
{
	uniform mat4 ProjInverse;
	uniform mat4 ViewInverse;
} u_Camera;


layout(constant_id = 0) const int MAX_RECURSION = 0;

struct RayPayload
{
	vec3 Color;
	float Distance;
	vec3 Normal;
	float Reflector;
};

layout(location = 0) rayPayloadEXT RayPayload rayPayload;

void main() 
{
	const vec2 pixelCenter = vec2(gl_LaunchIDEXT.xy) + vec2(0.5);
	const vec2 inUV = pixelCenter / vec2(gl_LaunchSizeEXT.xy);
	vec2 d = inUV * 2.0 - 1.0;

	vec4 origin = u_Camera.ViewInverse * vec4(0, 0, 0, 1);
	vec4 target = u_Camera.ProjInverse * vec4(d.x, d.y, 1, 1);
	vec4 direction = u_Camera.ViewInverse * vec4(normalize(target.xyz), 0);

	uint flags = gl_RayFlagsOpaqueEXT;
	uint mask = 0xFF;

	float tmin = 0.001;
	float tmax = 1000.0;

    vec3 color = vec3(0.0);

	for(int i = 0; i < MAX_RECURSION; i++)
	{
		traceRayEXT(TLAS,			//TLAS
					flags,			//Flags
					mask,			//Mask
					0,				//STB record offset
					0,				//STB record stride
					0,				//Miss index
					origin.xyz,		//Origin
					tmin,			//Min range
					direction.xyz,	//Direction
					tmax,			//Max range
					0				//Payload location
					);

		if(rayPayload.Distance < 0.0)
		{
			color += rayPayload.Color;
			break;
		}
		else if(rayPayload.Reflector <= 0.9f)
		{
			const vec4 hitPos = origin + direction * rayPayload.Distance;
			origin.xyz = hitPos.xyz + rayPayload.Normal * 0.001f;
			direction.xyz = reflect(direction.xyz, rayPayload.Normal);
			color += rayPayload.Color * rayPayload.Reflector;
		}
		else
		{
			color += rayPayload.Color * rayPayload.Reflector;
			break;
		}
	}

	imageStore(outputImage, ivec2(gl_LaunchIDEXT.xy), vec4(color, 1.0));
}

#type Miss

#version 460
#extension GL_EXT_ray_tracing : enable

struct RayPayload
{
	vec3 Color;
	float Distance;
	vec3 Normal;
	float Reflector;
};

layout(location = 0) rayPayloadInEXT RayPayload rayPayload;

layout(set = 1, binding = 0) uniform samplerCube u_EnvironmentMap;

void main() 
{
	vec3 unitDir = normalize(gl_WorldRayDirectionEXT);
	float t = 0.5 * (unitDir.y + 1.0);

	rayPayload.Color = texture(u_EnvironmentMap, unitDir).rgb;
	rayPayload.Distance = -1.0f;
	rayPayload.Normal = vec3(0.0);
	rayPayload.Reflector = 1.0f;
}

#type ClosestHit

#version 460
#extension GL_EXT_ray_tracing : enable
#extension GL_EXT_nonuniform_qualifier : enable

struct Vertex
{
	vec4 Position;
	vec4 Color;
	vec4 Normal;
	vec4 Tangent;
	vec4 Binormal;
	vec2 TextureCoords;
	vec2 Padding;
};

struct RayPayload
{
	vec3 Color;
	float Distance;
	vec3 Normal;
	float Reflector;
};


layout(location = 0) rayPayloadInEXT RayPayload rayPayload;

layout(set = 0, binding = 0) uniform accelerationStructureEXT topLevelAS;

hitAttributeEXT vec2 hitAttribs;

layout(set = 0, binding = 3) buffer Vertices { Vertex v[]; } vertices;
layout(set = 0, binding = 4) buffer Indices { uint i[]; } indices;


void main()
{
	ivec3 index = ivec3(indices.i[3 * gl_PrimitiveID], indices.i[3 * gl_PrimitiveID + 1], indices.i[3 * gl_PrimitiveID + 2]);
	const vec3 barycentricCoords = vec3(1.0 - hitAttribs.x - hitAttribs.y, hitAttribs.x, hitAttribs.y);
	
	//Vertices
	Vertex v0 = vertices.v[index.x];
	Vertex v1 = vertices.v[index.y];
	Vertex v2 = vertices.v[index.z];

	//Interpolate
	vec4 position = normalize(v0.Position * barycentricCoords.x + v1.Position * barycentricCoords.y + v2.Position * barycentricCoords.z);
	vec4 color = normalize(v0.Color * barycentricCoords.x + v1.Color * barycentricCoords.y + v2.Color * barycentricCoords.z);
	vec3 normal = normalize(v0.Normal.xyz * barycentricCoords.x + v1.Normal.xyz * barycentricCoords.y + v2.Normal.xyz * barycentricCoords.z);
	vec4 tangent = normalize(v0.Tangent * barycentricCoords.x + v1.Tangent * barycentricCoords.y + v2.Tangent * barycentricCoords.z);

	rayPayload.Color = color.rgb;
	rayPayload.Normal = normal;
	rayPayload.Reflector = max(dot(-gl_WorldRayDirectionEXT, normal), 0.0);
	rayPayload.Distance = gl_RayTmaxEXT;
}