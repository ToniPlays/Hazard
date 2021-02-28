#pragma once

#include <hzreditor.h>
#include "Performance.h"
#include "GUI/Library/Layout.h"
#include "GUI/Library/Input.h"

using namespace WindowLayout;
using namespace Hazard;

namespace WindowElement {

	Performance::Performance() : EditorWindow("Performance")
	{

	}
	Performance::~Performance()
	{

	}
	void Performance::Init()
	{
		bool found = false;
		context = &Core::HazardLoop::GetModule<Rendering::RenderContext>(found);
		SetActive(found);
		HZR_ASSERT(!found, "Performance can't start without RenderContext");
	}
	void Performance::OnWindowRender()
	{

		stats = Hazard::Core::HazardLoop::GetModule<Rendering::RenderEngine>().GetStats();

		Layout::Table(2, true);
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
		Layout::Text(context->GetRenderer().c_str());

		Layout::TableNext();
		Layout::Text("Drawcalls");
		Layout::TableNext();
		Layout::Text(std::to_string(stats.drawCalls).c_str());

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
		
		Layout::TableNext();
		Layout::Text("VSync");
		Layout::TableNext();
		static bool vsync = context->GetWindow().GetWindowInfo().VSync;
		Input::Checkbox("##VSYNC", vsync);
		context->GetWindow().SetVSync(vsync);

		Layout::EndTable();
	}
}