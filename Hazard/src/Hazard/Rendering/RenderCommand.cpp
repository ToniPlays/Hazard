#pragma once

#include <hzrpch.h>
#include "RenderCommand.h"

namespace Hazard::Rendering {

	RenderEngine RenderCommand::engine;
	RenderStats RenderCommand::stats;

	void RenderCommand::Init()
	{
		engine = Application::GetModule<RenderEngine>();
	}
	void RenderCommand::ResetStats()
	{
		stats.draws = 0;
		stats.quads = 0;
		stats.vertices = 0;
		stats.indices = 0;
	}
}
