#include "RenderGraph.h"
#include "spdlog/fmt/fmt.h"
#include <MathCore.h>

namespace HazardRenderer
{
	void RenderGraph::SetInput(const std::string& input, void* data, uint64_t size)
	{
		uint32_t index = m_InputResourceBindings.at(input);
		m_InputResources[index].InputData = Buffer(data, size);
	}

	void RenderGraph::Execute(Ref<RenderCommandBuffer> commandBuffer)
	{
		Timer timer;
		for (const auto& stage : m_Stages)
		{
			for (uint64_t i = 0; i < stage.InputCount; i++)
			{
				InputResource& resource = stage.pInputs[i];
				if (resource.UsageFlags & INPUT_RESOURCE_INSTRUCTIONS)
				{
					uint64_t inputIndex = m_InputResourceBindings[resource.Name];
					Buffer& data = m_InputResources[inputIndex].InputData;
					uint64_t offset = 0;

					while (offset < data.Size)
					{
						GraphInstruction& instruction = data.Read<GraphInstruction>(offset);
						offset += sizeof(GraphInstruction);

						if (instruction.Flags & INSTRUCTION_BIND_VERTEX_BUFFER)
						{
							//Vertex buffer reference
							ResourceReference& ref = GetResourceReference(stage, instruction.DataSource, instruction.Source.VertexBufferIndex);
							commandBuffer->SetVertexBuffer(ref.VertexBuffer, instruction.Destination.BindingIndex);
							continue;
						}

						if (instruction.Flags & INSTRUCTION_BIND_UNIFORM_BUFFER)
						{
							//TODO: This is a mess
							//Uniform buffer reference
							ResourceReference& bufferRef = GetResourceReference(stage, instruction.DataSource, instruction.Source.UniformBufferIndex);
							//Descriptorset reference
							ResourceReference& setRef = GetResourceReference(stage, instruction.DataDestination, instruction.Destination.DescriptorSetIndex);
							setRef.DescriptorSet->Write(instruction.Destination.BindingIndex, bufferRef.UniformBuffer);
							continue;
						}

						if (instruction.Flags & INSTRUCTION_BIND_PIPELINE)
						{
							ResourceReference& pipelineRef = GetResourceReference(stage, instruction.DataSource, instruction.Source.PipelineIndex);
							commandBuffer->SetPipeline(pipelineRef.Pipeline);
							continue;
						}
						if (instruction.Flags & INSTRUCTION_PUSH_CONSTANTS)
						{
							ResourceReference& dataRef = GetResourceReference(stage, instruction.DataSource, instruction.Source.PushConstantIndex);
							commandBuffer->PushConstants(*dataRef.PushConstantBuffer, instruction.Destination.Offset, 0);
							continue;
						}

						if (instruction.Flags & INSTRUCTION_BIND_DESCRIPTOR_SET)
						{
							InputResource& bufferSource = stage.pInputs[instruction.DataSource];
							Buffer& data = m_InputResources[instruction.DataSource].InputData;

							//Descriptorset reference
							ResourceReference& ref = data.Read<ResourceReference>(instruction.Source.DescriptorSetIndex * sizeof(ResourceReference));
							commandBuffer->SetDescriptorSet(ref.DescriptorSet, instruction.Destination.DescriptorSetIndex);
							continue;
						}

						if (instruction.Flags & INSTRUCTION_DRAW)
						{
							if (instruction.Flags & INSTRUCTION_NO_SOURCE)
							{
								commandBuffer->Draw(instruction.Destination.DrawCount, nullptr);
								continue;
							}

							ResourceReference& indexBufferRef = GetResourceReference(stage, instruction.DataSource, instruction.Source.IndexBufferIndex);
							commandBuffer->Draw(instruction.Destination.DrawCount, indexBufferRef.IndexBuffer);
							continue;
						}
					}
				}
			}
		}
		m_ExecutionTime = timer.ElapsedMillis();
	}

	Ref<RenderGraph> RenderGraph::Create(RenderGraphCreateInfo* createInfo)
	{
		return new RenderGraph(createInfo);
	}

	RenderGraph::RenderGraph(RenderGraphCreateInfo* createInfo) : m_DebugName(createInfo->DebugName)
	{
		for (uint64_t i = 0; i < createInfo->StageCount; i++)
		{
			RenderGraphStage& stage = createInfo->pStages[i];

			for (uint64_t j = 0; j < stage.InputCount; j++)
			{
				InputResource& resource = stage.pInputs[j];
				m_InputResources.push_back({ resource, Buffer() });
			}
			for (uint64_t j = 0; j < stage.InputImageCount; j++)
			{
				ImageDependency& image = stage.pInputImages[j];
				m_ImageDependencies[image.Name] = image;
			}
			for (uint64_t j = 0; j < stage.OutputImageCount; j++)
			{
				ImageDependency& image = stage.pOutputImages[j];
				m_ImageDependencies[image.Name] = image;
			}
		}

		CreateGraphTimeline(createInfo);
	}
	void RenderGraph::CreateGraphTimeline(RenderGraphCreateInfo* createInfo)
	{
		m_Stages.clear();
		std::vector<RenderGraphStage*> stagesToAdd;
		std::vector<RenderGraphStage*> addedStages;

		stagesToAdd.resize(createInfo->StageCount);
		for (uint64_t i = 0; i < createInfo->StageCount; i++)
			stagesToAdd[i] = &createInfo->pStages[i];

		while (!stagesToAdd.empty())
		{
			for (RenderGraphStage* stage : stagesToAdd)
			{
				bool missingDependencies = false;

				for (uint64_t i = 0; i < stage->DependencyCount; i++)
				{
					//Search trough dependencies
					RenderGraphStage* dependency = stage->pDependencies;
					missingDependencies = std::find(addedStages.begin(), addedStages.end(), dependency) == addedStages.end();
				}
				if (missingDependencies) continue;

				//Dependencies are added
				addedStages.emplace_back(stage);
				stagesToAdd.erase(std::find(stagesToAdd.begin(), stagesToAdd.end(), stage));
				break;
			}
		}
		for (RenderGraphStage* stage : addedStages)
		{
			if (stage->InputCount > 0)
			{
				InputResource* resources = stage->pInputs;
				stage->pInputs = new InputResource[stage->InputCount];

				for (uint64_t i = 0; i < stage->InputCount; i++)
				{
					InputResource* resource = &resources[i];
					m_InputResourceBindings[resource->Name] = i;
					stage->pInputs[i] = m_InputResources[i].Resource;
				}
			}
			m_Stages.push_back(*stage);
		}
	}
	ResourceReference& RenderGraph::GetResourceReference(const RenderGraphStage& stage, uint32_t inputIndex, uint64_t resourceIndex)
	{
		//Get input resource data
		InputResource& bufferSource = stage.pInputs[inputIndex];
		uint64_t inputBindingIndex = m_InputResourceBindings[bufferSource.Name];
		Buffer& data = m_InputResources[inputBindingIndex].InputData;

		return data.Read<ResourceReference>(resourceIndex * sizeof(ResourceReference));
	}
}
