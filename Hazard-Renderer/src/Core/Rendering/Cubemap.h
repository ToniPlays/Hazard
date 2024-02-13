#pragma once

#include "Core/Rendering/Pipeline.h"
#include "Image.h"
#include "Buffer/Buffer.h"

#include "glm/glm.hpp"

namespace HazardRenderer 
{
	struct CubemapCreateInfo
	{
		std::string DebugName;
		uint32_t Width;
		uint32_t Height;
		uint32_t MaxMips = 1;
		ImageUsage Usage = ImageUsage::None;
		ImageFormat Format = ImageFormat::None;
		Buffer Data;
	};

	class Cubemap: public Image {
	public:

		virtual ImageType GetType() const override { return ImageType::Cubemap; }
		virtual void RegenerateMips() = 0;
		static Ref<Cubemap> Create(CubemapCreateInfo* createInfo);

	};
}
