#pragma once

#include "Hazard/Rendering/Pipeline/Buffers.h"

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
			HZR_CORE_ASSERT(!GetVertexBuffer(name), "RenderLibrary already contains {0} VertexBuffer", name);
			m_VertexBuffers[name] = buffer;
		};
		static void AddIndexBuffer(Ref<IndexBuffer> buffer) {
			std::string& name = buffer->GetDebugName();
			HZR_CORE_ASSERT(!GetIndexBuffer(name), "RenderLibrary already contains {0} IndexBuffer", name);
			m_IndexBuffers[name] = buffer;
		};
		static void AddUniformBuffer(Ref<UniformBuffer> buffer) {
			std::string& name = buffer->GetName();
			HZR_CORE_ASSERT(!GetUniformBuffer(name), "RenderLibrary already contains {0} UniformBuffer", name);
			m_UniformBuffers[name] = buffer;
		};
		static bool HasVertexBuffer(const std::string& name) { return GetVertexBuffer(name); }
		static bool HasIndexBuffer(const std::string& name) { return GetIndexBuffer(name); }
		static bool HasUniformBuffer(const std::string& name) { return GetUniformBuffer(name); }

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
		
	private:
		inline static std::unordered_map<std::string, Ref<VertexBuffer>> m_VertexBuffers;
		inline static std::unordered_map<std::string, Ref<IndexBuffer>> m_IndexBuffers;
		inline static std::unordered_map<std::string, Ref<UniformBuffer>> m_UniformBuffers;

	};
}