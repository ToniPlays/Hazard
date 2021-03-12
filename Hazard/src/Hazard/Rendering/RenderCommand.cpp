#pragma once

#include <hzrpch.h>
#include "RenderCommand.h"

namespace Hazard::Rendering {

	RenderContext* RenderCommand::context;
	RenderEngine* RenderCommand::engine;
	RenderStats RenderCommand::stats;

	void RenderCommand::Init()
	{
		engine = &Application::GetModule<RenderEngine>();
		context = &Application::GetModule<RenderContext>();
	}
	void RenderCommand::ResetStats()
	{
		stats.draws = 0;
		stats.quads = 0;
		stats.vertices = 0;
		stats.indices = 0;
	}
	void RenderCommand::DrawIndexed(VertexArray* vao, uint32_t count)
	{
		stats.draws++;
		stats.indices += count;
		context->GetContext().DrawIndexed(vao, count);
	}
}
