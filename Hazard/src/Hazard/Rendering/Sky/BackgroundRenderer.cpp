#pragma once

#include <hzrpch.h>
#include "BackgroundRenderer.h"



namespace Hazard::Rendering {

	void ColorBackground::Render(glm::mat4 transform)
	{

	}
	SkyboxBackground::SkyboxBackground() 
	{
		m_Skybox = new Skybox();
		m_Skybox->SetCubemapTexture(nullptr);
	}
	SkyboxBackground::SkyboxBackground(CubemapTexture* texture)
	{
		m_Skybox = new Skybox();
		m_Skybox->SetCubemapTexture(texture);
	}
	SkyboxBackground::~SkyboxBackground() 
	{
		
	}
	void SkyboxBackground::Render(glm::mat4 transform)
	{
		m_Skybox->Render(transform);
	}
}
