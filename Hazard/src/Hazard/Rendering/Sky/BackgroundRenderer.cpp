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

		Ref<EnvinronmentMap>& envMap = RenderUtils::Create<EnvinronmentMap>("res/textures/comfy_cafe_1k.hdr");
		m_Skybox->SetCubemapTexture(static_cast<Ref<CubemapTexture>>(envMap));
	}

	SkyboxBackgroundRenderer::~SkyboxBackgroundRenderer()
	{
		m_Skybox->GetCubemapTexture()->Unbind();
		m_Skybox->GetCubemapTexture().Reset();
	}

	void SkyboxBackgroundRenderer::Render(glm::mat4 view, glm::mat4 projection)
	{
		RenderContextCommand::ClearFrame(m_Color);
		
		m_Skybox->Render(projection * glm::inverse(glm::mat4(glm::mat3(view))));
	}
	void SkyboxBackgroundRenderer::SetCubemap(Ref<CubemapTexture>& texture)
	{
		m_Skybox->SetCubemapTexture(texture);
	}
	Ref<Texture2D> SkyboxBackgroundRenderer::GetRaw() {
		Ref<EnvinronmentMap> envMap = static_cast<Ref<EnvinronmentMap>>(m_Skybox->GetCubemapTexture());
		return envMap->GetTexture();
	}
}
