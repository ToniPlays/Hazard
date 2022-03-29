#pragma once

#include "Camera.h"
#include "RawMesh.h"
#include "Backend/Core/Pipeline/Pipeline.h"
#include <vector>
#include <unordered_map>

namespace HazardRenderer 
{
	struct WorldSettings 
	{
		Camera RenderingCamera;
	};

	struct InstancedMesh 
	{
		Ref<RawMesh> Mesh;
		std::vector<glm::mat4> Transforms;
	};

	struct PipelineRenderable
	{
		std::unordered_map<uint64_t, std::vector<InstancedMesh>> MeshInstances;
	};
	struct DrawList 
	{
		WorldSettings Settings;

		std::unordered_map<uint64_t, PipelineRenderable> Geometry;
	};
}