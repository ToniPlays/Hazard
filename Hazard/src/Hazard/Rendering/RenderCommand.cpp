#pragma once
#include <hzrpch.h>
#include "RenderCommand.h"
#include "Hazard/RenderContext/RenderContextCommand.h"


namespace Hazard::Rendering 
{
	RenderAPI RenderCommand::s_Api;
	RenderEngine* RenderCommand::m_Engine;

	void RenderCommand::DrawIndexed(VertexArray* vertexArray, uint32_t size)
	{

	}
}
