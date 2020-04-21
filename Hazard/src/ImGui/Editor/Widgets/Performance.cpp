#pragma once
#include <hzrpch.h>
#include "Performance.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Core/Application.h"
#include "ImGui/Editor/GuiLayer.h"
#include "ImGui/Editor/EditorGUI.h"
#include "glad/glad.h"

namespace Hazard {

	void Performance::OnRender() const
	{
		if (!isLayerOpen) return;
		ImGui::Begin("Perfomance");

		if (ImGui::CloseButton(ImGui::GetID("Perf_1"), ImVec2(2, 2))) {
			GuiLayer* layer = (GuiLayer*)this;
			layer->CloseLayer();
		}

		if (ImGui::Button("Change clear color")) {
			EditorGUI::OpenPicker([](Color color) {
				Application::Get().GetWindow().SetClearColor(color);
			}, Application::Get().GetWindow().GetClearColor());
		}

		ImGui::SameLine(175);
		static bool vsync = false;
		ImGui::Checkbox("VSync", &vsync);

		Hazard::Application::Get().GetWindow().SetVSync(vsync);

		std::stringstream ss;
		ss << "Frametime " << Hazard::Time::unscaledDeltaTime * 1000 << " ms";
		ImGui::Text(ss.str().c_str());
		ss.str("");

		ImGui::SameLine(200);

		ImGui::Text(ss.str().c_str());
		ss.str("");

		ss << "FPS " << 1 / Hazard::Time::unscaledDeltaTime;
		ImGui::Text(ss.str().c_str());

		ss.str("");
		ss << "Renderer: " << glGetString(GL_RENDERER);
		ImGui::Text(ss.str().c_str());

		ImGui::End();
	}

}