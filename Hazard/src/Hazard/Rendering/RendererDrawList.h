#pragma once

#include "UtilityCore.h"
#include "WorldRenderer.h"
#include "Backend/Core/Pipeline/RenderPass.h"

namespace Hazard 
{
	

	struct RenderingCamera 
	{
		std::string DebugName;
		glm::mat4 ViewProjection = glm::mat4(1.0f);
		glm::vec3 Position = glm::vec3(0.0f);
		Ref<HazardRenderer::RenderPass> RenderPass;
		GeometryInclude GeometryFlags = Geometry_All;
	};

	struct RendererDrawList
	{
		std::vector<RenderingCamera> RenderingCameras;
		
	};
}