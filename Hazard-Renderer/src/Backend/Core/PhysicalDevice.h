#pragma once

#include "Core.h"
#include <glm/glm.hpp>

namespace HazardRenderer 
{
	struct PhysicalDeviceLimits 
	{
		uint32_t MaxTextureUnits;
		glm::vec<2, uint32_t> MaxFrameBufferSize;

		uint32_t MaxRecursionDepth = 0;
		uint32_t ShaderGroupHandleSize = 0;
		uint32_t TimestampPeriod = 0;
	};

	struct PhysicalDeviceCapabilities
	{
		bool Discrete = false;
        bool LowPower = false;
		bool Swapchain = false;
		
		//Diagnostic
		bool DiagnosticCheckpoint = false;
		bool DiagnosticConfig = false;
		bool DebugMarker = false;

		//RT
		bool AccelerationStructures = false;
		bool RayTracingPipeline = false;
		bool RayQuery = false;
	};


	class PhysicalDevice : public RefCount
	{
	public:
		virtual ~PhysicalDevice() {};
		virtual std::string GetDeviceName() = 0;
		virtual const PhysicalDeviceLimits& GetDeviceLimits() const = 0;
		virtual const PhysicalDeviceCapabilities& GetDeviceCababilities() const = 0;

		virtual bool SupportsRaytracing() const = 0;
	};
}
