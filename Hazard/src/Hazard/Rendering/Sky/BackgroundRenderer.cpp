#pragma once

#include <hzrpch.h>
#include "BackgroundRenderer.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

namespace Hazard::Rendering {

	void ColorBackgroundRenderer::Render(glm::mat4 view, glm::mat4 projection)
	{
		RenderContextCommand::ClearFrame(m_Color);
	}
	SkyboxBackgroundRenderer::SkyboxBackgroundRenderer() 
	{
		m_Skybox = new Skybox();

		EnvinronmentMap* envMap = RenderUtils::Create<EnvinronmentMap>("res/textures/comfy_cafe_1k.hdr");
		m_Skybox->SetCubemapTexture(envMap);
	}

	void SkyboxBackgroundRenderer::Render(glm::mat4 view, glm::mat4 projection)
	{
		RenderContextCommand::ClearFrame(m_Color);
		
		m_Skybox->Render(projection * glm::inverse(glm::mat4(glm::mat3(view))));
	}
	void SkyboxBackgroundRenderer::SetCubemap(CubemapTexture* texture)
	{
		m_Skybox->SetCubemapTexture(texture);
	}
	Texture2D* SkyboxBackgroundRenderer::GetRaw() {
		EnvinronmentMap* envMap = (EnvinronmentMap*)m_Skybox->GetCubemapTexture();
		return envMap->GetTexture();
	}
}
