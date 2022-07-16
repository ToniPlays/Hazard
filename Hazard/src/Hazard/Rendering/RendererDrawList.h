#pragma once

#include "UtilityCore.h"
#include "WorldRenderer.h"
#include "Backend/Core/Pipeline/RenderPass.h"

namespace Hazard 
{
	

	struct RenderingCamera 
	{
		std::string DebugName;
		glm::mat4 ViewProjection;
		glm::vec3 Position;
		Ref<HazardRenderer::RenderPass> RenderPass;
		GeometryInclude GeometryFlags;
	};

	struct RendererDrawList
	{
		std::vector<RenderingCamera> RenderingCameras;
	};
}