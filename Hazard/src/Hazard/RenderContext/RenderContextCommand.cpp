
#include <hzrpch.h>
#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext* RenderContextCommand::m_Context;

	void RenderContextCommand::SetTitle(const char* title)
	{
		if (m_Context == nullptr) return;
		m_Context->GetWindow().SetWindowTitle(title);
	}
	float RenderContextCommand::GetWindowWidth()
	{
		return m_Context->GetWindow().GetWidth();
	}
	float RenderContextCommand::GetWindowHeight()
	{
		return m_Context->GetWindow().GetHeight();
	}
	bool RenderContextCommand::IsFullscreen()
	{
		return m_Context->GetWindow().IsFullscreen();
	}
	void RenderContextCommand::SetFullscreen(bool fullscreen)
	{
		m_Context->GetWindow().SetFullscreen(fullscreen);
	}
	bool RenderContextCommand::IsVsync()
	{
		return m_Context->GetWindow().IsVSync();
	}
	void RenderContextCommand::SetVsync(bool vsync)
	{
		m_Context->GetWindow().SetVSync(vsync);
	}
	void RenderContextCommand::SetDebugCallback(ErrorCallback& callback)
	{
		m_Context->GetContext().SetErrorListener(callback);
	}
	void RenderContextCommand::SetClearColor(const Color& color)
	{
		m_Context->GetContext().SetClearColor({ color.r, color.g, color.b, color.a });
	}
}
