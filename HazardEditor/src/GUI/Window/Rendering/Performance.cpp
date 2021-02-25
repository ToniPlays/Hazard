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
		ss << Time::unscaledDeltaTime * 1000 << "ms";
		Layout::Text(ss.str().c_str());

		Layout::TableNext();
		Layout::Text("Platform");
		Layout::TableNext();
		Layout::Text(context->GetWindow().GetWindowInfo().Platform);

		Layout::TableNext();
		Layout::Text("Renderer");
		Layout::TableNext();
		Layout::Text(context->GetRenderer().c_str());
		static bool vsync = context->GetWindow().GetWindowInfo().VSync;
		Input::Checkbox("VSync", vsync);
		context->GetWindow().SetVSync(vsync);

		Layout::EndTable();
	}
}