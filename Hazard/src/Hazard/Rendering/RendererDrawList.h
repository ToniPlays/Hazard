#pragma once

#include "UtilityCore.h"
#include "RenderResources.h"
#include "Renderers/WorldRenderer.h"
#include "Backend/Core/Pipeline/RenderPass.h"
#include "Backend/Core/Pipeline/Pipeline.h"
#include "Backend/Core/Pipeline/Buffers.h"
#include "Environment/EnvironmentMap.h"

namespace Hazard
{
	struct DrawListStat 
	{
		uint64_t QuadCount;
		uint64_t MeshCount;
		uint64_t Vertices;
		uint64_t Indices;
		uint64_t DrawCalls;
	};

    struct MeshInstance
    {
        InstanceTransform Transform;
    };

	struct RawMesh
	{
        std::vector<MeshInstance> Instances;
		Ref<HazardRenderer::VertexBuffer> VertexBuffer;
		Ref<HazardRenderer::IndexBuffer> IndexBuffer;
        
		int Flags;
		size_t IndexCount;
	};

	struct PipelineData 
	{
		size_t Count;
	};

	struct EnvironmentData
	{
		Ref<EnvironmentMap> Map;
		float SkylightIntensity;
		float EnvironmentLod;
	};
    struct LightSource
    {
        glm::vec3 Color;
        float Intensity;
    };

	struct DirectionalLightSource : LightSource
	{
		glm::vec3 Direction;
	};

    struct PointLightSource : LightSource
    {
        float Radius;
    };

    struct SpotLightSource : LightSource
    {
        float Radius;
        float ConeAngle;
    };

	struct RenderingCamera
	{
		std::string DebugName;
		glm::mat4 ViewProjection = glm::mat4(1.0f);
		glm::mat4 Projection = glm::mat4(1.0f);
		glm::mat4 View = glm::mat4(1.0f);
		glm::vec3 Position = glm::vec3(0.0f);
		Ref<HazardRenderer::RenderPass> RenderPass;
		GeometryInclude GeometryFlags = Geometry_All;
	};

    using MeshDrawList = std::unordered_map<Material*, std::unordered_map<HazardRenderer::VertexBuffer*, RawMesh>>;

    //Draw list for single world context
	struct RendererDrawList
	{
		Ref<WorldRenderer> WorldRenderer;
		MeshDrawList MeshList;
		
        EnvironmentData Environment;
		std::vector<DirectionalLightSource> DirectionalLights;
        std::vector<PointLightSource> PointLights;
        std::vector<SpotLightSource> SpotLights;
		std::unordered_map<HazardRenderer::Pipeline*, std::vector<PipelineData>> Pipelines;

		DrawListStat Stats;
	};
}
