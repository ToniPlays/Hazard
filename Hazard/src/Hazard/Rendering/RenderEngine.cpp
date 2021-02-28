#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"
#include "RenderUtils.h"

namespace Hazard::Rendering {

	RenderEngine::RenderEngine() : Module("RenderEngine")
	{
		SetActive(true);
	}
	RenderEngine::~RenderEngine()
	{
		Flush();
	}
	void RenderEngine::PreInit()
	{
		
	}
	void RenderEngine::Init()
	{
		bool found = false;
		context = &Core::HazardLoop::GetModule<RenderContext>(found);
		HZR_CORE_ASSERT(found, "RenderEngine cannot start without RenderContext");
		SetActive(found);

		vertexArray = RenderUtils::Create<VertexArray>();
		shader = RenderUtils::Create<Shader>("res/shaders/standard.glsl");

		VertexBuffer* buffer = RenderUtils::Create<VertexBuffer>((uint32_t)1000);
		vertexArray->AddBuffer(buffer);

		float positions[12] = {
			-0.5f, -0.5f, 0.5f,
			-0.5f,  0.5f, 0.5f,
			 0.5f,  0.5f, 0.5f,
			 0.5f, -0.5f, 0.5f
		};

		buffer->SetLayout({ {ShaderDataType::Float3, "position" } });
		buffer->SetData(positions, 12);

		uint32_t indices[6] = {
			0, 1, 2,
			0, 2, 3
		};

		buffer->SetData(positions, sizeof(positions));
		IndexBuffer* indexBuffer = RenderUtils::Create<IndexBuffer>();
		indexBuffer->SetData(indices, 6);
		vertexArray->SetIndexBuffer(indexBuffer);

	}
	void RenderEngine::Flush()
	{
		delete context; 
	}
	void RenderEngine::SceneRender(ECS::Scene& scene)
	{
		if (renderTarget == nullptr) 
			return;

		renderTarget->Bind();
		shader->Bind();

		context->GetWindow().GetContext()->ClearFrame("#222222");
		context->GetWindow().GetContext()->DrawIndexed(vertexArray);

		shader->Unbind();
		renderTarget->Unbind();
	}
}