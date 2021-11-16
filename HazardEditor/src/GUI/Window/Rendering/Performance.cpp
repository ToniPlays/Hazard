#pragma once

#include "Performance.h"
#include "Library/Layout/Layout.h"
#include "Library/Input.h"

using namespace WindowLayout;
using namespace Hazard;

namespace WindowElement {

	Performance::Performance() : EditorWindow(ICON_FK_BAR_CHART " Performance")
	{
		SetActive(false);
	}
	void Performance::Init()
	{
		context = &Application::GetModule<Rendering::RenderContext>();
	}
	void Performance::OnWindowRender()
	{
		Rendering::RenderStats& stats = Rendering::RenderCommand::GetStats();

		Layout::Table(2, true);
		Layout::SetColumnWidth(115);
		Layout::Text("Frametime");
		Layout::TableNext();

		std::stringstream ss;
		ss << Math::Round(Time::s_UnscaledDeltaTime * 1000, 3) << "ms";
		Layout::Text(ss.str().c_str());
		ss.str("");
		Layout::TableNext();
		Layout::Text("FPS");
		Layout::TableNext();

		ss << Math::Round(1.0f / Time::s_UnscaledDeltaTime, 3) << " fps";
		Layout::Text(ss.str().c_str());

		Layout::TableNext();
		Layout::Text("Platform");
		Layout::TableNext();
		Layout::Text(context->GetWindow().GetWindowInfo().Platform);

		Hazard::Rendering::DeviceSpec spec = context->GetWindow().GetWindowInfo().deviceSpecs;

		Layout::TableNext();
		Layout::Text("Renderer");
		Layout::TableNext();
		Layout::Text(spec.Name);

		Layout::TableNext();
		Layout::Text("Rendering API");
		Layout::TableNext();
		Layout::Text(spec.Renderer);

		Layout::TableNext();
		Layout::Text("Drawcalls");
		Layout::TableNext();
		Layout::Text(std::to_string(stats.DrawCalls));

		Layout::TableNext();
		Layout::Text("Quads");
		Layout::TableNext();
		Layout::Text(StringUtil::FormatNumber(stats.QuadCount));

		Layout::TableNext();
		Layout::Text("Vertices");
		Layout::TableNext();
		Layout::Text(StringUtil::FormatNumber(stats.VertexCount));

		Layout::TableNext();
		Layout::Text("Indices");
		Layout::TableNext();
		Layout::Text(StringUtil::FormatNumber(stats.IndexCount));

		Layout::EndTable();
	}
}