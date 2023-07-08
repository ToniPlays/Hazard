#pragma once

#include "Backend/Core/Image2D.h"
#include "Backend/Core/Sampler.h"
#include "Backend/Core/GPUBuffer.h"
#include "Backend/Core/Pipeline.h"
#include "Backend/Core/DescriptorSet.h"
#include <span>

namespace HazardRenderer
{
	enum InputResourceFlags
	{
		INPUT_RESOURCE_INSTRUCTIONS =	BIT(0),
		INPUT_RESOURCE_RESOURCE_LIST =	BIT(1),
	};
	enum GraphInstructionType
	{
		INSTRUCTION_BIND_VERTEX_BUFFER =	BIT(0),
		INSTRUCTION_BIND_BUFFER =			BIT(1),
		INSTRUCTION_BIND_UNIFORM_BUFFER =	BIT(2),
		INSTRUCTION_BIND_PIPELINE =			BIT(3),
		INSTRUCTION_BIND_DESCRIPTOR_SET =	BIT(4),
		INSTRUCTION_PUSH_CONSTANTS =		BIT(5),
		INSTRUCTION_DRAW =					BIT(6),
		INSTRUCTION_COMPUTE =				BIT(7),
		INSTRUCTION_NO_SOURCE =				BIT(8),
	};

	struct GraphInstruction
	{
		union ResourceIndex
		{
			uint32_t VertexBufferIndex;
			uint32_t IndexBufferIndex;
			uint32_t UniformBufferIndex;
			uint32_t PipelineIndex;
			uint32_t DescriptorSetIndex;
			uint32_t SamplerIndex;
			uint32_t ImageIndex;
			uint32_t PushConstantIndex;
			uint32_t BindingIndex;
			uint32_t DrawCount;
			uint32_t Offset;
		};

		uint32_t Flags = 0;
		ResourceIndex Source = { .BindingIndex = 0 };
		ResourceIndex Destination = { .BindingIndex = 0 };

		uint32_t DataSource = 0;
		uint32_t DataDestination = 0;
		uint32_t DataSize = 0;

		void SetVertexBuffer(uint32_t dataSource, uint32_t bufferIndex, uint32_t bindingIndex)
		{
			Flags = INSTRUCTION_BIND_VERTEX_BUFFER;
			DataSource = dataSource;
			Source.VertexBufferIndex = bufferIndex;
			Destination.BindingIndex = bindingIndex;
		};
		void SetPipeline(uint32_t dataSource, uint32_t pipelineIndex)
		{
			Flags = INSTRUCTION_BIND_PIPELINE;
			DataSource = dataSource;
			Source.PipelineIndex = pipelineIndex;
		};
		void SetDescriptor(uint32_t dataSource, uint32_t descriptorIndex, uint32_t bindingIndex)
		{
			Flags = INSTRUCTION_BIND_DESCRIPTOR_SET;
			DataSource = dataSource;
			Source.DescriptorSetIndex = descriptorIndex;
			Destination.BindingIndex = bindingIndex;
		}
		void PushConstants(uint32_t dataSource, uint32_t pushConstantIndex, uint32_t size)
		{

			Flags = INSTRUCTION_PUSH_CONSTANTS;
			DataSource = dataSource;
			Source.PushConstantIndex = pushConstantIndex;
			DataSize = size;
		}
		void Draw(uint32_t drawCount)
		{
			Flags = INSTRUCTION_DRAW | INSTRUCTION_NO_SOURCE;
			Destination.DrawCount = drawCount;
		};
		void Draw(uint32_t drawCount, uint32_t bufferSource, uint32_t indexBufferIndex)
		{
			Flags = INSTRUCTION_DRAW;
			DataSource = bufferSource;
			Source.IndexBufferIndex = indexBufferIndex;
			Destination.DrawCount = drawCount;
		};
	};


	union ResourceReference
	{
		GPUBuffer* VertexBuffer;
		GPUBuffer* IndexBuffer;
		GPUBuffer* UniformBuffer;
		Pipeline* Pipeline;
		Image* Image;
		Sampler* Sampler;
		Buffer* PushConstantBuffer;
		DescriptorSet* DescriptorSet;
	};


	struct InputResource
	{
		std::string Name;
		uint32_t UsageFlags;
	};
	struct StageDescriptor
	{
		std::string DebugName;
		uint32_t Set = 0;
		Ref<DescriptorSet> DescriptorSet;
	};

	struct RenderGraphStage
	{
		uint32_t DependencyCount = 0;
		RenderGraphStage* pDependencies = nullptr;
		uint32_t InputCount = 0;
		InputResource* pInputs = nullptr;
		uint32_t DescriptorCount = 0;
		StageDescriptor* pStageDescriptors = nullptr;
	};

	struct RenderGraphCreateInfo
	{
		std::string DebugName;
		uint64_t StageCount;
		RenderGraphStage* pStages;
	};
}