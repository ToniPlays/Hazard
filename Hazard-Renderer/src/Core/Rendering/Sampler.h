#pragma once
#include "Image.h"

namespace HazardRenderer
{
	struct SamplerCreateInfo
	{
		std::string DebugName;
		FilterMode MinFilter = FilterMode::Linear;
		FilterMode MagFilter = FilterMode::Linear;
		ImageWrap Wrapping = ImageWrap::Repeat;
	};

	class Sampler : public RefCount
	{
	public:
		virtual ~Sampler() = default;

		virtual const std::string& GetDebugName() const = 0;
		virtual FilterMode GetMinFilter() const = 0;
		virtual FilterMode GetMagFilter() const = 0;
		virtual ImageWrap GetWrapping() const = 0;


		static Ref<Sampler> Create(SamplerCreateInfo* info);
	};
}
