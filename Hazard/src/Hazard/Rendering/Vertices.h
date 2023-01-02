#pragma once

#include <glm/glm.hpp>
#include "Backend/Core/Pipeline/BufferLayout.h"

namespace Hazard
{
	struct Vertex3D
	{
		glm::vec4 Position = { 0, 0, 0, 1.0 };
		glm::vec4 Color = Color::White;
		glm::vec4 Normals = { 0, 0, 0, 0 };
		glm::vec4 Tangent = { 0, 0, 0, 0 };
		glm::vec4 Binormal = { 0, 0, 0, 0 };
		glm::vec2 TexCoords = { 0, 0 };

		static HazardRenderer::BufferLayout Layout() {
			using namespace HazardRenderer;
			return {
				{ "a_Position",			ShaderDataType::Float4, PerVertex },
				{ "a_Color",			ShaderDataType::Float4, PerVertex },
				{ "a_Normal",			ShaderDataType::Float4, PerVertex },
				{ "a_Tangent",			ShaderDataType::Float4, PerVertex },
				{ "a_Binormal",			ShaderDataType::Float4, PerVertex },
				{ "a_TextureCoords",	ShaderDataType::Float2, PerVertex },
				{ "a_MRow0",			ShaderDataType::Float4, PerInstance },
                { "a_MRow0",            ShaderDataType::Float4, PerInstance },
                { "a_MRow0",            ShaderDataType::Float4, PerInstance }
			};
		}
	};

	struct QuadVertex 
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoords;
		float TextureIndex;

		static HazardRenderer::BufferLayout Layout() 
		{
			using namespace HazardRenderer;
			return {
				{ "a_Position",		ShaderDataType::Float3 },
				{ "a_Color",		ShaderDataType::Float4 },
				{ "a_TextureCoords",ShaderDataType::Float2 },
				{ "a_TextureIndex",	ShaderDataType::Float  }
			};
		}
	};
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		static HazardRenderer::BufferLayout Layout() 
		{
			using namespace HazardRenderer;
			return {
				{ "a_Position",		ShaderDataType::Float3 },
				{ "a_Color",		ShaderDataType::Float4 }
			};
		}
	};
}
