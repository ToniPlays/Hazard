#pragma once

#include <hzrpch.h>
#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext* RenderContextCommand::m_Context;

	void RenderContextCommand::Init()
	{
		m_Context = &Application::GetModule<RenderContext>();
	}
	void RenderContextCommand::SetDepthTest(DepthFunc type)
	{
		m_Context->GetContext().SetDepthTest(type);
	}
	bool RenderContextCommand::IsVsync()
	{
		return m_Context->GetWindow().IsVSync();
	}
	void RenderContextCommand::SetVsync(bool vsync)
	{
		m_Context->GetWindow().SetVSync(vsync);
	}
	void RenderContextCommand::ClearFrame(glm::vec4 color)
	{
		m_Context->GetWindow().GetContext()->ClearFrame(color);
	}
	void RenderContextCommand::SetDebugCallback(ErrorCallback& callback)
	{
		m_Context->GetContext().SetErrorListener(callback);
	}
}
