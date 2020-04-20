#pragma once
#include <hzrpch.h>
#include "GuiStats.h"
#include "imgui.h"
#include "Core/Application.h"
#include "ImGui/EditorGUI.h"

namespace Hazard {

	void GuiStats::OnRender() const
	{
		ImGui::Begin("Perfomance", &show);
		static bool vsync = false;
		ImGui::Checkbox("VSync", &vsync);

		Application::Get().GetWindow().SetVSync(vsync);

		std::stringstream ss;
		ss << "Frametime " << Time::unscaledDeltaTime * 1000 << " ms";
		ImGui::Text(ss.str().c_str());

		if (ImGui::Button("Change clear color")) {
			GuiLayer* layer = (GuiLayer*)this;
			EditorGUI::OpenPicker(layer);
		}

		ImGui::End();
	}
	void GuiStats::OnGetColor(Color color)
	{
		HZR_CORE_WARN("Changing color");
		Application::Get().GetWindow().SetClearColor(color);
	}
	
}