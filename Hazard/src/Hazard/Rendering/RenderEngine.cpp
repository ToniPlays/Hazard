#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"

#include "Hazard/Rendering/RenderUtils.h"
#include "Mesh/VertexData.h"
#include "RenderCommand.h"

#include "Mesh/MeshFactory.h"

namespace Hazard::Rendering {

	RenderEngine::RenderEngine() : Module("RenderEnginwe")
	{
		SetActive(true);
		std::cout << "RenderEngine()" << std::endl;
	}
	RenderEngine::~RenderEngine()
	{

	}
	void RenderEngine::PreInit()
	{
		
	}
	void RenderEngine::Init()
	{
		renderer2D = new Renderer2D(&RenderContextCommand::GetContext());
		renderer2D->Init(35000);
		CubemapTexture* texture = RenderUtils::Create<CubemapTexture>("res/textures/sea-", ".jpg");

		skybox = new Skybox();
		skybox->SetCubemapTexture(texture);
		RenderCommand::Init();
	}
	void RenderEngine::Close()
	{
		renderer2D->Close();
	}

	void RenderEngine::BeginRendering(Camera camera)
	{
		RenderCommand::ResetStats();
		renderTarget->Bind();

		RenderContextCommand::ClearFrame(camera.clearColor);
		viewProjection = camera.projection * glm::inverse(camera.view);
		cameraPosition = camera.position;
		
		skybox->Render(camera.projection * glm::inverse(glm::mat4(glm::mat3(camera.view))));

		renderer2D->BeginScene(viewProjection);
		renderer2D->BeginBatch();
	}
		
	void RenderEngine::EndRendering()
	{
		renderer2D->Flush();
		renderTarget->Unbind();
	}
}