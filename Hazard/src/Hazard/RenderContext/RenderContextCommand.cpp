#pragma once

#include <hzrpch.h>
#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext RenderContextCommand::context;

	void RenderContextCommand::Init()
	{
		context = Application::GetModule<RenderContext>();
	}
	bool RenderContextCommand::IsVsync()
	{
		return context.GetWindow().IsVSync();
	}
	void RenderContextCommand::SetVsync(bool vsync)
	{
		context.GetWindow().SetVSync(vsync);
	}
}
