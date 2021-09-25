#pragma once

#include <hzrpch.h>
#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext* RenderContextCommand::m_Context;

	/*void RenderContextCommand::SetDepthTest(DepthFunc type)
	{
		m_Context->GetContext().SetDepthTest(type);
	}*/
	void RenderContextCommand::SetTitle(const char* title)
	{
		if (m_Context == nullptr) return;
		m_Context->GetWindow().SetWindowTitle(title);
	}
	bool RenderContextCommand::IsVsync()
	{
		return m_Context->GetWindow().IsVSync();
	}
	void RenderContextCommand::SetVsync(bool vsync)
	{
		m_Context->GetWindow().SetVSync(vsync);
	}
	void RenderContextCommand::SwapBuffers()
	{
		m_Context->GetWindow().GetContext()->SwapBuffers();
	}
	void RenderContextCommand::SetDebugCallback(ErrorCallback& callback)
	{
		m_Context->GetContext().SetErrorListener(callback);
	}
	void RenderContextCommand::SetClearColor(Color color)
	{
		m_Context->GetContext().SetClearColor(color);
	}
}
