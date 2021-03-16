#pragma once

#include <hzrpch.h>
#include "BackgroundRenderer.h"



namespace Hazard::Rendering {

	void ColorBackground::Render(glm::mat4 transform)
	{

	}
	SkyboxBackground::SkyboxBackground() 
	{
		skybox = new Skybox();
		skybox->SetCubemapTexture(nullptr);
	}
	SkyboxBackground::SkyboxBackground(CubemapTexture* texture)
	{
		skybox = new Skybox();
		skybox->SetCubemapTexture(texture);
	}
	SkyboxBackground::~SkyboxBackground() 
	{
		
	}
	void SkyboxBackground::Render(glm::mat4 transform)
	{
		skybox->Render(transform);
	}
}
