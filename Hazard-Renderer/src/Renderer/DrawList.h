#pragma once

#include "UID.h"
#include "Camera.h"
#include "Mesh/RawMesh.h"
#include "Backend/Core/Pipeline/Pipeline.h"
#include <vector>
#include <unordered_map>

namespace HazardRenderer 
{
	struct WorldSettings 
	{
		Camera* RenderingCamera;
		Ref<RenderPass> TargetRenderPass;
	};
	struct TransformData 
	{
		glm::vec4 MRow[3];
	};

	struct InstancedMesh 
	{
		Ref<RawMesh> RawMesh;
		std::vector<TransformData> Transforms;
	};

	struct PipelineRenderable
	{
		Ref<Pipeline> Pipeline;
		std::unordered_map<HazardUtility::UID, InstancedMesh> MeshInstances;
	};
	struct DrawList 
	{
		WorldSettings Settings;

		std::unordered_map<HazardUtility::UID, PipelineRenderable> Geometry;

		uint32_t Quads = 0;

	};
}