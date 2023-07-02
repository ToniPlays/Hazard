#pragma once

#include "DescriptorSetLayout.h"
#include "Image2D.h"
#include "Sampler.h"
#include "GPUBuffer.h"

namespace HazardRenderer
{

	struct DescriptorSetCreateInfo
	{
		uint32_t Set;
		DescriptorSetLayout* pLayout;
	};

	class DescriptorSet : public RefCount
	{
	public:
		virtual ~DescriptorSet() = default;
		virtual void Write(uint32_t binding, Ref<Image> image, Ref<Sampler> sampler, bool updateAll = false) = 0;
		virtual void Write(uint32_t binding, Ref<GPUBuffer> buffer, bool updateAll = false) = 0;

		static Ref<DescriptorSet> Create(DescriptorSetCreateInfo* createInfo);
	};
}
