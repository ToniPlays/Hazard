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
		context = &Core::HazardLoop::GetModule<Rendering::RenderContext>();
	}
	void Performance::OnWindowRender()
	{
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
		Layout::Text("VSync");
		Layout::TableNext();
		static bool vsync = context->GetWindow().GetWindowInfo().VSync;
		Input::Checkbox("##VSYNC", vsync);
		context->GetWindow().SetVSync(vsync);

		Layout::EndTable();
	}
}