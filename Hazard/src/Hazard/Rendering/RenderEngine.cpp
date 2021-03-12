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
		meshModel = MeshFactory::LoadMesh("c:/dev/Hazard/HazardEditor/res/Models/shuttle.fbx");
		MeshFactory::LoadMesh("c:/dev/Hazard/HazardEditor/res/Models/cube.obj");
		MeshFactory::LoadMesh("c:/dev/Hazard/HazardEditor/res/Models/backpack.obj");
		MeshFactory::LoadMesh("c:/dev/Hazard/HazardEditor/res/Models/cube.obj");
		MeshFactory::LoadMesh("c:/dev/Hazard/HazardEditor/res/Models/shuttle.fbx");
		MeshFactory::LoadMesh("c:/dev/Hazard/HazardEditor/res/Models/shuttle.fbx");

		meshModel->GetMaterial().GetShader().Bind();
		meshModel->GetMaterial().GetShader().SetUniformInt("envMap", 0);
		meshModel->GetMaterial().GetShader().Unbind();
		//meshModel = MeshFactory::LoadCube();

		HZR_CORE_ASSERT((meshModel != nullptr), "Mesh failed to load");
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
		glm::mat4 viewProjection = camera.projection * glm::inverse(camera.view);
		
		skybox->Render(camera.projection * glm::inverse(glm::mat4(glm::mat3(camera.view))));

		meshModel->GetMaterial().GetShader().Bind();
		meshModel->GetMaterial().GetShader().SetUniformMat4("viewProjection", viewProjection);
		meshModel->GetMaterial().GetShader().SetUniformVec3("cameraPos", camera.position);
		meshModel->Render();


		renderer2D->BeginScene(viewProjection);
		renderer2D->BeginBatch();
	}
		
	void RenderEngine::EndRendering()
	{
		renderer2D->Flush();
		renderTarget->Unbind();
	}
}