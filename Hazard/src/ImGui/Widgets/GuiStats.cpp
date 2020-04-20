#pragma once
#include <hzrpch.h>
#include "GuiStats.h"
#include "imgui.h"
#include "Core/Application.h"
#include "ImGui/EditorGUI.h"
#include "glad/glad.h"

namespace Hazard {

	void GuiStats::OnRender() const
	{
		ImGui::Begin("Perfomance", &show);

		if (ImGui::Button("Change clear color")) {
			GuiLayer* layer = (GuiLayer*)this;
			EditorGUI::OpenPicker(layer, Application::Get().GetWindow().GetClearColor());
		}
		ImGui::SameLine(175);
		static bool vsync = false;
		ImGui::Checkbox("VSync", &vsync);

		Application::Get().GetWindow().SetVSync(vsync);

		std::stringstream ss;
		ss << "Frametime " << Time::unscaledDeltaTime * 1000 << " ms";
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ImGui::SameLine(200);

		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "FPS " << 1 / Time::unscaledDeltaTime;
		ImGui::Text(ss.str().c_str());

		ss.str("");
		ss << "Renderer: " << glGetString(GL_RENDERER);
		ImGui::Text(ss.str().c_str());

		ImGui::End();
	}
	void GuiStats::OnGetColor(Color color)
	{
		Application::Get().GetWindow().SetClearColor(color);
	}
	
}