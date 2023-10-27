#pragma once

#include "DescriptorSetLayout.h"
#include "Image2D.h"
#include "Sampler.h"
#include "GPUBuffer.h"
#include "Pipeline.h"

namespace HazardRenderer
{

	struct DescriptorSetCreateInfo
	{
		std::string DebugName;
		uint32_t Set;
		DescriptorSetLayout* pLayout;
		uint32_t Flags;
	};

	class DescriptorSet : public RefCount
	{
	public:
		virtual void Write(uint32_t binding, uint32_t index, Ref<Image> image, Ref<Sampler> sampler, bool updateAll = false) = 0;
		virtual void Write(uint32_t binding, Ref<GPUBuffer> buffer, bool updateAll = false) = 0;

		static Ref<DescriptorSet> Create(DescriptorSetCreateInfo* createInfo);
	};
}
