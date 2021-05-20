#pragma once

#include <hzrpch.h>
#include "BackgroundRenderer.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

namespace Hazard::Rendering {

	SkyboxBackgroundRenderer::SkyboxBackgroundRenderer() 
	{
		HZR_PROFILE_FUNCTION();
		m_Skybox = new Skybox();

		CubemapCreateInfo createInfo;
		createInfo.datatype = TextureDataType::HDR;
		createInfo.width = 2048;
		createInfo.height = 2048;
		createInfo.sides = { { CubeSide::All, "res/textures/modern_buildings_8k.hdr"} };

		/*std::vector<CubemapSide> sides = {
			{ CubeSide::Top,	"res/textures/sea-top.jpg"},
			{ CubeSide::Bottom, "res/textures/sea-bottom.jpg"},
			{ CubeSide::Left,	"res/textures/sea-left.jpg"},
			{ CubeSide::Right,	"res/textures/sea-right.jpg"},
			{ CubeSide::Front,	"res/textures/sea-front.jpg"},
			{ CubeSide::Back,	"res/textures/sea-back.jpg"}
		};*/

		Ref<CubemapTexture>& envMap = RenderUtils::Create<CubemapTexture>(createInfo);
		m_Skybox->SetCubemapTexture(envMap);
	}

	SkyboxBackgroundRenderer::~SkyboxBackgroundRenderer()
	{
		m_Skybox->GetCubemapTexture()->Unbind();
		m_Skybox->GetCubemapTexture().Reset();
	}

	void SkyboxBackgroundRenderer::Render()
	{
		m_Skybox->Render();
	}
	void SkyboxBackgroundRenderer::SetCubemap(Ref<CubemapTexture>& texture)
	{
		m_Skybox->SetCubemapTexture(texture);
	}
	Ref<Texture2D> SkyboxBackgroundRenderer::GetRaw() {
		return m_Skybox->GetCubemapTexture()->GetInfo().cubeSides.at(0);
	}
}
