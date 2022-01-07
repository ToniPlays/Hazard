#pragma once

#include "Hazard/Rendering/Pipeline/Buffers.h"
#include "Hazard/Rendering/Pipeline/Pipeline.h"

namespace Hazard::Rendering {
	class RenderLibrary 
	{
	public:
		static void Clear() {
			m_VertexBuffers.clear();
			m_IndexBuffers.clear();
			m_UniformBuffers.clear();
		};
		static void AddVertexBuffer(Ref<VertexBuffer> buffer) {
			std::string& name = buffer->GetDebugName();
			HZR_CORE_ASSERT(!HasVertexBuffer(name), "RenderLibrary already contains {0} VertexBuffer", name);
			m_VertexBuffers[name] = buffer;
		};
		static void AddIndexBuffer(Ref<IndexBuffer> buffer) {
			std::string& name = buffer->GetDebugName();
			HZR_CORE_ASSERT(!HasIndexBuffer(name), "RenderLibrary already contains {0} IndexBuffer", name);
			m_IndexBuffers[name] = buffer;
		};
		static void AddUniformBuffer(Ref<UniformBuffer> buffer) {
			std::string& name = buffer->GetName();
			HZR_CORE_ASSERT(!HasUniformBuffer(name), "RenderLibrary already contains {0} UniformBuffer", name);
			m_UniformBuffers[name] = buffer;
		};
		static void AddPipeline(Ref<Pipeline> pipeline) {
			std::string name = pipeline->GetSpecifications().DebugName;
			HZR_CORE_ASSERT(!GetPipeline(name), "")
			m_Pipelines[name] = pipeline;
		};
		static bool HasVertexBuffer(const std::string& name) { return GetVertexBuffer(name); }
		static bool HasIndexBuffer(const std::string& name) { return GetIndexBuffer(name); }
		static bool HasUniformBuffer(const std::string& name) { return GetUniformBuffer(name); }
		static bool HasPipeline(const std::string& name) { return GetPipeline(name); }

		static Ref<VertexBuffer> GetVertexBuffer(const std::string& name) {
			if (m_VertexBuffers.find(name) == m_VertexBuffers.end()) 
				return nullptr;
			return m_VertexBuffers[name];
		};
		static Ref<IndexBuffer> GetIndexBuffer(const std::string& name) {
			if (m_IndexBuffers.find(name) == m_IndexBuffers.end())
				return nullptr;
			return m_IndexBuffers[name];
		};
		static Ref<UniformBuffer> GetUniformBuffer(const std::string& name) {
			if (m_UniformBuffers.find(name) == m_UniformBuffers.end())
				return nullptr;
			return m_UniformBuffers[name];
		};
		static Ref<Pipeline> GetPipeline(const std::string& name) {
			if (m_Pipelines.find(name) == m_Pipelines.end())
				return nullptr;
			return m_Pipelines[name];
		};
		
	private:
		inline static std::unordered_map<std::string, Ref<VertexBuffer>> m_VertexBuffers;
		inline static std::unordered_map<std::string, Ref<IndexBuffer>> m_IndexBuffers;
		inline static std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffers;
		inline static std::unordered_map<std::string, Ref<Pipeline>> m_Pipelines;

	};
}