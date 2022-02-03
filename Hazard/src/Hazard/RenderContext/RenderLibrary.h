#pragma once

#include "Pipeline/Buffers.h"
#include "Pipeline/Pipeline.h"
#include "Hazard/Assets/AssetManager.h"
#include <hzrpch.h>

namespace Hazard::Rendering {
	class RenderLibrary 
	{
	public:
		static void Clear() 
		{
			HZR_CORE_INFO("Clearing render library");
			s_RuntimeResources.clear();
		};
		static void AddVertexBuffer(Ref<VertexBuffer> buffer) {
			std::string& name = "VBO_" + buffer->GetDebugName();
			HZR_CORE_ASSERT(!HasVertexBuffer(buffer->GetDebugName()), "RenderLibrary already contains {0} VertexBuffer", name);
			AssetManager::AddRuntimeResource(buffer);
			s_RuntimeResources[name] = buffer;
		};
		static void AddIndexBuffer(Ref<IndexBuffer> buffer) {
			std::string& name = "IBO_" + buffer->GetDebugName();
			HZR_CORE_ASSERT(!HasIndexBuffer(buffer->GetDebugName()), "RenderLibrary already contains {0} IndexBuffer", name);
			s_RuntimeResources[name] = buffer;
			AssetManager::AddRuntimeResource(buffer);
		};
		static void AddUniformBuffer(Ref<UniformBuffer> buffer) {
			std::string& name = "UBO_" + buffer->GetName();
			HZR_CORE_ASSERT(!HasUniformBuffer(buffer->GetName()), "RenderLibrary already contains {0} UniformBuffer", name);
			s_RuntimeResources[name] = buffer;
			AssetManager::AddRuntimeResource(buffer);
		};
		static void AddPipeline(Ref<Pipeline> pipeline) {
			std::string name = "RP_" + pipeline->GetSpecifications().DebugName;
			HZR_CORE_ASSERT(!GetPipeline(pipeline->GetSpecifications().DebugName), "No pipeline {0}", name);
			s_RuntimeResources[name] = pipeline;
			AssetManager::AddRuntimeResource(pipeline);
		};
		static bool HasVertexBuffer(const std::string& name) { return GetVertexBuffer(name); }
		static bool HasIndexBuffer(const std::string& name) { return GetIndexBuffer(name); }
		static bool HasUniformBuffer(const std::string& name) { return GetUniformBuffer(name); }
		static bool HasPipeline(const std::string& name) { return GetPipeline(name); }

		static Ref<VertexBuffer> GetVertexBuffer(const std::string& name) {
			std::string key = "VBO_" + name;
			if (s_RuntimeResources.find(key) == s_RuntimeResources.end())
				return nullptr;
			return AssetManager::GetRuntimeResource<VertexBuffer>(s_RuntimeResources[key]->GetHandle()).As<VertexBuffer>();
		};
		static Ref<IndexBuffer> GetIndexBuffer(const std::string& name) {
			std::string key = "IBO_" + name;
			if (s_RuntimeResources.find(key) == s_RuntimeResources.end())
				return nullptr;
			return AssetManager::GetRuntimeResource<IndexBuffer>(s_RuntimeResources[key]->GetHandle()).As<IndexBuffer>();
		};
		static Ref<UniformBuffer> GetUniformBuffer(const std::string& name) {
			std::string key = "UBO_" + name;
			if (s_RuntimeResources.find(key) == s_RuntimeResources.end())
				return nullptr;
			return AssetManager::GetRuntimeResource<UniformBuffer>(s_RuntimeResources[key]->GetHandle()).As<UniformBuffer>();
		};
		static Ref<Pipeline> GetPipeline(const std::string& name) {
			std::string key = "RP_" + name;
			if (s_RuntimeResources.find(key) == s_RuntimeResources.end())
				return nullptr;
			return AssetManager::GetRuntimeResource<Pipeline>(s_RuntimeResources[key]->GetHandle()).As<Pipeline>();
		};
		
	private:
		inline static std::unordered_map<std::string, Ref<RuntimeResource>> s_RuntimeResources;

	};
}