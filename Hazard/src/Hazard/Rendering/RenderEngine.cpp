#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"

#include "Hazard/Rendering/RenderUtils.h"
#include "2D/QuadData.h"
#include "RenderCommand.h"

namespace Hazard::Rendering {

	RenderEngine::RenderEngine() : Module("RenderEngine")
	{
		SetActive(true);
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

		std::vector<std::string> faces;

		CubemapTexture* texture = RenderUtils::Create<CubemapTexture>("res/textures/starfield_", ".png");

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

		skybox->Render(camera.projection * glm::inverse(glm::mat4(glm::mat3(camera.view))));
		
		renderer2D->BeginScene(camera.projection * glm::inverse(camera.view));
		renderer2D->BeginBatch();
	}
	void RenderEngine::EndRendering()
	{
		renderer2D->Flush();
		renderTarget->Unbind();
	}
}