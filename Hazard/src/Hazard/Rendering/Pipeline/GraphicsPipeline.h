#pragma once

#include <glm/glm.hpp>

namespace Hazard::Rendering 
{

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
	enum CullFace { FrontFace = 0, BackFace, LeftFace, RightFace };
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
	};


	struct GraphicsPipelineCreateInfo
	{
		InputAssembly* inputAssembly;
		PipelineViewport* viewport;

		PipelineRasterizer* rasterizer;
	};

	struct GraphicsPipelineInfo 
	{
		uint32_t rawDepthFunc;
		PipelineViewport viewport;
	};

	class GraphicsPipeline {
	public:
		virtual ~GraphicsPipeline() {};

		virtual void Bind() = 0;
	};
}