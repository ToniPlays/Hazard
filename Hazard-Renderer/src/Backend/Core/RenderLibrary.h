#pragma once

#include "Buffers.h"

#include <unordered_map>
#include <string>

namespace HazardRenderer 
{
	class RenderLibrary 
	{
	public:
		static bool HasUniformBuffer(const std::string& name)
		{
			return s_UniformBuffers.find(name) != s_UniformBuffers.end();
		}
		static void AddUniformBuffer(Ref<UniformBuffer> uniformBuffer) 
		{
			s_UniformBuffers[uniformBuffer->GetName()] = uniformBuffer;
		}
		static Ref<UniformBuffer> GetUniformBuffer(const std::string& name) 
		{
			return s_UniformBuffers[name];
		}

	private:
		inline static std::unordered_map<std::string, Ref<UniformBuffer>> s_UniformBuffers;
	};
}
