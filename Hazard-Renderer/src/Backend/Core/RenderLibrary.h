#pragma once

#include "UtilityCore.h"
#include "Pipeline/Buffers.h"
#include "Pipeline/Pipeline.h"

namespace HazardRenderer 
{
	class RenderLibrary 
	{
	public:
		static bool HasVertexBuffer(const std::string& name) 
		{ 
			return s_VertexBuffers.find(name) != s_VertexBuffers.end(); 
		};
		static bool HasIndexBuffer(const std::string& name)
		{ 
			return s_IndexBuffers.find(name) != s_IndexBuffers.end();
		};
		static bool HasUniformBuffer(const std::string& name)
		{
			return s_UniformBuffes.find(name) != s_UniformBuffes.end();
		};
		static bool HasPipeline(const std::string& name)
		{ 
			return s_Pipelines.find(name) != s_Pipelines.end();
		};
		static void AddUniformBuffer(Ref<UniformBuffer> uniformBuffer) {
			s_UniformBuffes[uniformBuffer->GetName()] = uniformBuffer;
		}
		static void AddPipeline(Ref<Pipeline> pipeline) {
			s_Pipelines[pipeline->GetSpecifications().DebugName] = pipeline;
		}

		static Ref<VertexBuffer> GetVertexBuffer(const std::string& name) {
			return s_VertexBuffers[name];
		};
		static Ref<IndexBuffer> GetIndexBuffer(const std::string& name) {
			return s_IndexBuffers[name];
		};
		static Ref<UniformBuffer> GetUniformBuffer(const std::string& name) {
			return s_UniformBuffes[name];
		};
		static Ref<Pipeline> GetPipeline(const std::string& name) {
			return s_Pipelines[name];
		};

	private:
		inline static std::unordered_map<std::string, Ref<VertexBuffer>> s_VertexBuffers;
		inline static std::unordered_map<std::string, Ref<IndexBuffer>> s_IndexBuffers;
		inline static std::unordered_map<std::string, Ref<UniformBuffer>> s_UniformBuffes;
		inline static std::unordered_map<std::string, Ref<Pipeline>> s_Pipelines;
	};
}