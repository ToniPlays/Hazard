#pragma once

#include <hzrpch.h>
#include "RenderCommand.h"

namespace Hazard::Rendering {

	RenderContext* RenderCommand::s_Context;
	RenderEngine* RenderCommand::s_Engine;
	RenderStats RenderCommand::s_Stats;

	void RenderCommand::Init()
	{
		s_Engine  = &Application::GetModule<RenderEngine>();
		s_Context = &Application::GetModule<RenderContext>();
	}
	void RenderCommand::ResetStats()
	{
		s_Stats.draws = 0;
		s_Stats.quads = 0;
		s_Stats.vertices = 0;
		s_Stats.indices = 0;
	}
	void RenderCommand::DrawIndexed(VertexArray* vao, uint32_t count)
	{
		s_Stats.draws++;
		s_Stats.indices += count;
		s_Context->GetContext().DrawIndexed(*vao, count);
	}
}
