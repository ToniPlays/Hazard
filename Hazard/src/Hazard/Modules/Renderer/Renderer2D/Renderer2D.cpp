#pragma once

#include <hzrpch.h>
#include "Renderer2D.h"
#include "Hazard//Modules/Renderer/GlobalRenderer.h"

namespace Hazard {
	

	Renderer2D::Renderer2D()
	{
		planeMesh = MeshLoader::LoadPlane();
	}

	Renderer2D::~Renderer2D()
	{

	}

	void Renderer2D::Render()
	{
		GlobalRenderer::Draw(planeMesh);
	}

	void Renderer2D::OnResized(Event& e)
	{

	}
}