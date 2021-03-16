#pragma once

#include <hzreditor.h>
#include "Performance.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Input.h"

using namespace WindowLayout;
using namespace Hazard;

namespace WindowElement {

	Performance::Performance() : EditorWindow(ICON_FK_BAR_CHART " Performance")
	{

	}
	Performance::~Performance()
	{

	}
	void Performance::Init()
	{
		bool found = false;
		context = &Application::GetModule<Rendering::RenderContext>(found);
		SetActive(found);
		HZR_ASSERT(!found, "Performance can't start without RenderContext");
	}
	void Performance::OnWindowRender()
	{

		Rendering::RenderStats& stats = Rendering::RenderCommand::GetStats();

		Layout::Table(2, true);
		Layout::SetColumnWidth(115);
		Layout::Text("Frametime");
		Layout::TableNext();

		std::stringstream ss;
		ss << Math::Round(Time::unscaledDeltaTime * 1000, 3) << "ms";
		Layout::Text(ss.str().c_str());
		ss.str("");
		Layout::TableNext();
		Layout::Text("FPS");
		Layout::TableNext();

		ss << Math::Round(1.0f / Time::unscaledDeltaTime, 3) << " fps";
		Layout::Text(ss.str().c_str());

		Layout::TableNext();
		Layout::Text("Platform");
		Layout::TableNext();
		Layout::Text(context->GetWindow().GetWindowInfo().Platform);

		Layout::TableNext();
		Layout::Text("Renderer");
		Layout::TableNext();
		Layout::Text(context->GetContext().GetDevice().c_str());

		Layout::TableNext();
		Layout::Text("Rendering API");
		Layout::TableNext();
		Layout::Text(context->GetContext().GetVersion().c_str());

		Layout::TableNext();
		Layout::Text("Drawcalls");
		Layout::TableNext();
		Layout::Text(std::to_string(stats.draws).c_str());

		Layout::TableNext();
		Layout::Text("Quads");
		Layout::TableNext();
		Layout::Text(Utility::StringUtil::FormatNumber(stats.quads).c_str());

		Layout::TableNext();
		Layout::Text("Vertices");
		Layout::TableNext();
		Layout::Text(Utility::StringUtil::FormatNumber(stats.vertices).c_str());

		Layout::TableNext();
		Layout::Text("Indices");
		Layout::TableNext();
		Layout::Text(Utility::StringUtil::FormatNumber(stats.indices).c_str());

		Layout::EndTable();
	}
}