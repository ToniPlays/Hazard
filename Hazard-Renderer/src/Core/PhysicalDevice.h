#pragma once

#include "Core.h"
#include <glm/glm.hpp>

namespace HazardRenderer 
{
	struct PhysicalDeviceLimits 
	{
		uint32_t MaxTextureUnits;
		glm::vec2 MaxFrameBufferSize;
	};


	class PhysicalDevice 
	{
	public:
		virtual ~PhysicalDevice() {};
		virtual std::string GetDeviceName() = 0;
		virtual const PhysicalDeviceLimits& GetDeviceLimits() const = 0;

		static Ref<PhysicalDevice> Create();
	};
}