#pragma once

#include <glm/glm.hpp>
#include "Hazard/Core/Ref.h"

namespace Hazard::Rendering 
{
	class Shader;

	enum DepthFunc {
		Never = 0,
		Less = 1,
		Equal = 2,
		LessOrEqual = 3,
		Greater = 4,
		NotEqual = 5,
		GreaterOrEqual = 6,
		Always = 7
	};

	enum InputTopology { TriangleList = 0 };
	enum CullFace { None = 0, FrontFace, BackFace, FontAndBackFace };
	enum ShaderType { VertexShader = 0, FragmentShader = 1, GeometryShader = 2, ComputeShader = 3 };
	enum ShaderFileType { Binary = 0, Source = 1 };

	struct PipelineViewport 
	{
		glm::vec2 offset = { 0, 0 };
		glm::vec2 size = { 0, 0 };
	};

	struct InputAssembly 
	{
		InputTopology topology;
	};

	struct PipelineShaderStage
	{
		std::string shaderFileName;
		ShaderType stage;
		ShaderFileType fileType = ShaderFileType::Source;
	};

	struct PipelineRasterizer 
	{
		DepthFunc depthFunc;
		CullFace cullFace;
	};

	struct GraphicsPipelineCreateInfo
	{
		InputAssembly* inputAssembly;
		PipelineViewport* viewport;
		PipelineRasterizer* rasterizer;

		PipelineShaderStage* stages;
		std::string shaderPipelineName;
		uint32_t stageCount;
	};

	struct GraphicsPipelineInfo 
	{
		std::string pipelineName;

		uint32_t rawDepthFunc;
		uint32_t rawCullFace;
		PipelineViewport viewport;
		Ref<Shader> pipelineShader;
	};

	class GraphicsPipeline {
	public:
		virtual ~GraphicsPipeline() {};

		virtual void Bind() = 0;
		virtual GraphicsPipelineInfo GetInfo() = 0;
		virtual Ref<Shader> GetShader() = 0;
	};
}