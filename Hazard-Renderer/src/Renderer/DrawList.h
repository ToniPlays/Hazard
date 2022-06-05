#pragma once

#include "UID.h"
#include "Camera.h"
#include "Mesh/RawMesh.h"
#include "Backend/Core/Pipeline/Pipeline.h"

#include <vector>
#include <unordered_map>
#include <map>

namespace HazardRenderer 
{
	struct WorldSettings 
	{
		Camera* RenderingCamera;
		Ref<RenderPass> TargetRenderPass;
	};
	struct TransformData 
	{
		glm::vec4 Color;
		glm::vec4 MRow[3];
	};

	struct InstancedMesh 
	{
		Ref<RawMesh> RawMesh;
		std::vector<TransformData> Transforms;
		uint32_t InstanceCount = 0;

		InstancedMesh() 
		{
			Transforms.reserve(1000);
		}
	};

	struct PipelineRenderable
	{
		Ref<Pipeline> Pipeline;
		std::map<UID, InstancedMesh> MeshInstances;
	};
	struct DrawList 
	{
		WorldSettings Settings;
		std::map<UID, PipelineRenderable> Geometry;
		uint32_t Quads = 0;
	};
}