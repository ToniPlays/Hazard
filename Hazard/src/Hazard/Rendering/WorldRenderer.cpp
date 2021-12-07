#pragma once
#include "hzrpch.h"
#include "WorldRenderer.h"

namespace Hazard::Rendering 
{
	void WorldRenderer::SetViewport(uint32_t width, uint32_t height) 
	{
		m_Settings.ViewportSize = { width, height };

		Invalidate();
	}
	void WorldRenderer::Invalidate() 
	{ 
		
	}
}