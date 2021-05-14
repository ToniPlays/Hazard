#pragma once

#include <hzrpch.h>
#include "BackgroundRenderer.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

namespace Hazard::Rendering {

	void ColorBackgroundRenderer::Render(glm::mat4 view, glm::mat4 projection)
	{

	}
	SkyboxBackgroundRenderer::SkyboxBackgroundRenderer() 
	{
		m_Skybox = new Skybox();

		CubemapCreateInfo createInfo;
		createInfo.datatype = TextureDataType::HDR;
		createInfo.width = 2048;
		createInfo.height = 2048;
		createInfo.sides = { { CubeSide::All, "res/textures/chapmans_drive_8k.hdr"} };

		Ref<CubemapTexture>& envMap = RenderUtils::Create<CubemapTexture>(createInfo);
		m_Skybox->SetCubemapTexture(envMap);
	}

	SkyboxBackgroundRenderer::~SkyboxBackgroundRenderer()
	{
		m_Skybox->GetCubemapTexture()->Unbind();
		m_Skybox->GetCubemapTexture().Reset();
	}

	void SkyboxBackgroundRenderer::Render(glm::mat4 view, glm::mat4 projection)
	{
		m_Skybox->Render(projection * glm::inverse(glm::mat4(glm::mat3(view))));
	}
	void SkyboxBackgroundRenderer::SetCubemap(Ref<CubemapTexture>& texture)
	{
		m_Skybox->SetCubemapTexture(texture);
	}
	Ref<Texture2D> SkyboxBackgroundRenderer::GetRaw() {
		return m_Skybox->GetCubemapTexture()->GetInfo().environmentRawTexture;
	}
}
