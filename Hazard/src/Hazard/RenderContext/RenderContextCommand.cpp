#pragma once

#include <hzrpch.h>
#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext* RenderContextCommand::context;

	void RenderContextCommand::Init()
	{
		context = &Application::GetModule<RenderContext>();
	}
	void RenderContextCommand::SetDepthTest(DepthTest type)
	{
		context->GetContext().SetDepthTest(type);
	}
	bool RenderContextCommand::IsVsync()
	{
		return context->GetWindow().IsVSync();
	}
	void RenderContextCommand::SetVsync(bool vsync)
	{
		context->GetWindow().SetVSync(vsync);
	}
	void RenderContextCommand::ClearFrame(glm::vec4 color)
	{
		context->GetWindow().GetContext()->ClearFrame(color);
	}
}
