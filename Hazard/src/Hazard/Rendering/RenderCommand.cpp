#pragma once

#include <hzrpch.h>
#include "RenderCommand.h"

namespace Hazard::Rendering {

	RenderEngine RenderCommand::engine;

	void RenderCommand::Init()
	{
		engine = Application::GetModule<RenderEngine>();
	}
}
