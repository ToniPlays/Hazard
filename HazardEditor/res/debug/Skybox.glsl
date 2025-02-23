#type Vertex
#version 450
#include "Uniforms/CameraUniform.glslh"



			layout(location = 0) out vec3 OUT_Position;
			

			vec3 quadPos[6] = vec3[](
				vec3( 1,  1, 0), vec3(-1, -1, 0), vec3(-1,  1, 0),
				vec3(-1, -1, 0), vec3( 1,  1, 0), vec3( 1, -1, 0)
			);

			void main() 
			{
				vec4 position = vec4(quadPos[gl_VertexIndex].xy, 1.0, 1.0);
				gl_Position = position;

				vec4 inversedPos = position;
				OUT_Position = (mat4(mat3(inverse(u_Camera.View))) * inverse(u_Camera.Projection) * inversedPos).xyz;
			}

#type Fragment
#version 450
#include "Utils/PostProcessing.glslh"

layout (set = 0, binding = 1) uniform samplerCube u_RadianceMap;


layout(push_constant, std140) uniform PushConstants
{	uniform float LodLevel;
	uniform float Intensity;
} CONSTANT;
			layout(location = 0) in vec3 IN_Position;
			

			layout(location = 0) out vec4 OUT_Color;
			layout(location = 1) out uint OUT_EntityID;
			

			const float gamma = 2.2;

			void main() 
			{
				int levelTexels = textureQueryLevels(u_RadianceMap);
				vec3 color = textureLod(u_RadianceMap, IN_Position, (CONSTANT.LodLevel * levelTexels)).rgb;
				color *= CONSTANT.Intensity;
				color = GammaCorrect(color, gamma);

				OUT_Color = vec4(ACESTonemap(color), 1.0);

				OUT_EntityID = 0;
			}

